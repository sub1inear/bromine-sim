"""Exports the active Fusion design to a bromine-sim robot.json."""
# pyright: reportAttributeAccessIssue=false
import traceback
import adsk.core
import adsk.fusion
import json
import os
import math
import re

app      = adsk.core.Application.get()
ui       = app.userInterface
handlers = []

COMMAND_ID = 'Bromine Sim Fusion Export'
FIELD_LEFT = -70.0
MERGE_TOLERANCE = 1.0
PORT_ORDER = ['motor', 'rotation', 'imu', 'distance']

DEVICE_NAMES = {
    'motor': ('motor', 'motors'),
    'wheel': ('wheel', 'wheels'),
    'imu': ('IMU', 'IMUs'),
    'distance': ('distance sensor', 'distance sensors'),
    'rotation': ('rotation sensor', 'rotation sensors'),
}

SETTINGS_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'settings.json')
DEFAULT_SETTINGS = {
    'exportPath': '',
    'mergeExisting': True,
    'turns': 0,
    'mass': 0.0,
    'gearset': 'blue',
    'motorRatio': 1.0,
    'excludeRegex': '',
    'previewExclusions': True,
    'fullWattMotorRegex': '11w',
    'halfWattMotorRegex': '5\\.5w',
    'omniWheelRegex': 'omni',
    'tractionWheelRegex': 'traction',
    'flexWheelRegex': 'flex',
    'imuRegex': 'imu|inertial',
    'distanceRegex': 'distance',
    'rotationRegex': 'rotation',
    'motorAngleOffset': 0.0,
    'wheelAngleOffset': 0.0,
    'caseSensitive': False,
}

class ExportError(Exception):
    pass

def loadSettings() -> dict:
    settings = dict(DEFAULT_SETTINGS)
    try:
        with open(SETTINGS_PATH) as file:
            saved = json.load(file)
        settings.update({k: v for k, v in saved.items() if k in DEFAULT_SETTINGS})
    except FileNotFoundError:
        pass
    except (OSError, ValueError, AttributeError) as e:
        app.log(f'Could not read {SETTINGS_PATH}, using defaults: {e}')
    return settings

def saveSettings(inputs):
    settings = {}
    for id in DEFAULT_SETTINGS:
        input = inputs.itemById(id)
        dropDown = adsk.core.DropDownCommandInput.cast(input)
        settings[id] = dropDown.selectedItem.name if dropDown else input.value
    with open(SETTINGS_PATH, 'w') as file:
        json.dump(settings, file, indent=4)
        file.write('\n')

def plural(count: int, singular: str, pluralForm: str) -> str:
    return f'{count} {singular if count == 1 else pluralForm}'

def joinList(items: list[str]) -> str:
    """Joins items as an English list with an Oxford comma."""
    if len(items) <= 2:
        return ' and '.join(items)
    return ', '.join(items[:-1]) + ', and ' + items[-1]

def cmToInches(cm: float) -> float:
    return cm / 2.54

def heading(x: float, y: float) -> float:
    """Angle of a direction in degrees, 0 is up, positive is clockwise."""
    return math.degrees(math.atan2(x, y))

def normalizeAxisAngle(angle: float) -> float:
    """Maps an angle of an undirected axis into [0, 90] U (270, 360)."""
    angle %= 360
    if 90 < angle <= 270:
        angle -= 180
    return round(angle, 4) % 360

def processRegex(regex: str, caseSensitive: bool):
    processedRegex = '(?!)' if regex == '' else regex
    flags = re.NOFLAG if caseSensitive else re.IGNORECASE
    return re.compile(processedRegex, flags)

class Frame:
    """Converts Fusion world coordinates (cm) to robot coordinates (in), centered and turned."""
    def __init__(self, centerX: float, centerY: float, turns: int):
        self.centerX = centerX
        self.centerY = centerY
        self.angle = turns * math.pi / 2

    def vector(self, x: float, y: float) -> tuple[float, float]:
        c, s = math.cos(self.angle), math.sin(self.angle)
        return x * c - y * s, x * s + y * c

    def point(self, p: adsk.core.Point3D) -> tuple[float, float]:
        x, y = self.vector(p.x - self.centerX, p.y - self.centerY)
        return cmToInches(x), cmToInches(y)

class Obb:
    """An occurrence's oriented bounding box in robot coordinates."""
    def __init__(self, occ, frame: Frame):
        obb = occ.orientedMinimumBoundingBox
        self.x, self.y = frame.point(obb.centerPoint)
        self.z = cmToInches(obb.centerPoint.z)
        self.axes = []
        for direction, extent in ((obb.lengthDirection, obb.length),
                                  (obb.widthDirection, obb.width),
                                  (obb.heightDirection, obb.height)):
            d = direction.copy()
            d.normalize()
            x, y = frame.vector(d.x, d.y)
            self.axes.append((x, y, d.z, cmToInches(extent)))

    def horizontalAxes(self):
        axes = [a for a in self.axes if abs(a[2]) < 0.7]
        return axes if axes else self.axes

    def longestHorizontalHeading(self) -> float:
        x, y, _, _ = max(self.horizontalAxes(), key=lambda a: a[3])
        return heading(x, y)

def getModelBbox(design, isIncluded) -> adsk.core.BoundingBox3D:
    root = design.rootComponent
    bbox = None
    extremes = {}

    def add(body, path):
        nonlocal bbox
        bb = body.boundingBox
        name = f'{path}/{body.name}' if path else body.name
        if bbox is None:
            bbox = adsk.core.BoundingBox3D.create(bb.minPoint.copy(), bb.maxPoint.copy())
            extremes.update({key: name for key in ('minX', 'maxX', 'minY', 'maxY', 'minZ', 'maxZ')})
            return
        for axis in 'XYZ':
            a = axis.lower()
            if getattr(bb.minPoint, a) < getattr(bbox.minPoint, a):
                extremes[f'min{axis}'] = name
            if getattr(bb.maxPoint, a) > getattr(bbox.maxPoint, a):
                extremes[f'max{axis}'] = name
        bbox.combine(bb)

    def walk(occ):
        if not isIncluded(occ):
            return
        for body in occ.bRepBodies:
            if body.isVisible:
                add(body, occ.fullPathName)
        for child in occ.childOccurrences:
            walk(child)

    for body in root.bRepBodies:
        if body.isVisible:
            add(body, '')
    for occ in root.occurrences:
        walk(occ)

    if bbox is None:
        raise ExportError('No visible bodies found in the design.')

    size = [cmToInches(getattr(bbox.maxPoint, a) - getattr(bbox.minPoint, a)) for a in 'xyz']
    app.log(f'Bounding box (world, in): X {size[0]:.2f}, Y {size[1]:.2f}, Z {size[2]:.2f}')
    for key, name in extremes.items():
        app.log(f'  {key} set by: {name}')
    return bbox

COM_TOLERANCE = 0.005

def distanceSensorAxis(occ, obb: 'Obb', frame: Frame) -> tuple[float, float, float]:
    """The sensor's lens axis, which runs along the middle-length side of its box, in robot coordinates.

    The axis points toward the heavier end. Returns (x, y, center of mass offset along it in inches).
    """
    middle = sorted(obb.axes, key=lambda a: a[3])[1]
    com = occ.getPhysicalProperties(adsk.fusion.CalculationAccuracy.HighCalculationAccuracy).centerOfMass
    comX, comY = frame.point(com)
    offset = (comX - obb.x, comY - obb.y, cmToInches(com.z) - obb.z)
    c = sum(offset[k] * middle[k] for k in range(3))
    sign = -1 if c < 0 else 1
    return sign * middle[0], sign * middle[1], abs(c)

def orientDistanceSensors(sensors: list[tuple[dict, str, tuple[float, float, float]]]):
    """Points each distance sensor out of its lens.

    The lens is on the same end (heavier or lighter) of every copy of a component, so the end is
    decided by whichever faces outward for most copies. This keeps sensors that face inward from
    being flipped outward.
    """
    groups: dict[str, list] = {}
    for device, componentName, axis in sensors:
        groups.setdefault(componentName, []).append((device, axis))
    for componentName, group in groups.items():
        score = sum(ax * d['x'] + ay * d['y'] for d, (ax, ay, c) in group if c >= COM_TOLERANCE)
        sign = 1 if score >= 0 else -1
        app.log(f'Distance sensors ({componentName}) face their {"heavier" if sign > 0 else "lighter"} end')
        for d, (ax, ay, c) in group:
            if c < COM_TOLERANCE:
                sensorSign = 1 if ax * d['x'] + ay * d['y'] >= 0 else -1
                app.log(f'  Could not tell front from back at ({d["x"]}, {d["y"]}); facing it outward')
            else:
                sensorSign = sign
            d['angle'] = round(heading(sensorSign * ax, sensorSign * ay), 4) % 360

def assignIds(devices: list[dict]):
    """Links each rotation sensor to its nearest wheel, and drive motors/wheels by side."""
    wheels = [d for d in devices if d['type'] == 'wheel']
    trackingWheels = []
    nextId = 2
    for rotation in (d for d in devices if d['type'] == 'rotation'):
        candidates = [w for w in wheels if w not in trackingWheels]
        if candidates:
            wheel = min(candidates, key=lambda w: math.dist((w['x'], w['y']), (rotation['x'], rotation['y'])))
            wheel['id'] = nextId
            trackingWheels.append(wheel)
        rotation['id'] = nextId
        nextId += 1

    for d in devices:
        if d['type'] == 'motor' or (d['type'] == 'wheel' and d not in trackingWheels):
            d['id'] = 0 if d['x'] < 0 else 1

MERGED_KEYS = {
    'motor': ['port', 'id', 'role', 'reversed', 'ratio', 'gearset'],
    'wheel': ['id'],
    'rotation': ['port', 'id', 'ratio', 'reversed'],
    'imu': ['port'],
    'distance': ['port'],
}

def mergeExisting(devices: list[dict], oldDevices: list[dict]) -> int:
    """Copies non-geometric settings from the nearest previously exported device. Returns the match count."""
    unmatched = [d for d in oldDevices if isinstance(d, dict) and 'x' in d and 'y' in d]
    matches = 0
    for d in devices:
        candidates = [o for o in unmatched if o.get('type') == d['type']]
        if not candidates:
            continue
        old = min(candidates, key=lambda o: math.dist((o['x'], o['y']), (d['x'], d['y'])))
        if math.dist((old['x'], old['y']), (d['x'], d['y'])) > MERGE_TOLERANCE:
            continue
        unmatched.remove(old)
        matches += 1
        for key in MERGED_KEYS[d['type']]:
            if key in old and key in d:
                d[key] = old[key]
    return matches

def assignPorts(devices: list[dict]):
    used = {d['port'] for d in devices if d.get('port') is not None}
    free = [p for p in range(1, 22) if p not in used]
    unassigned = [d for d in devices if 'port' in d and d['port'] is None]
    unassigned.sort(key=lambda d: (PORT_ORDER.index(d['type']), -d['y'], d['x']))
    if len(unassigned) > len(free):
        raise ExportError(f'Not enough smart ports: {plural(len(unassigned), "device needs", "devices need")} a port '
                          f'but only {plural(len(free), "is", "are")} free.')
    for d, port in zip(unassigned, free):
        d['port'] = port

class CommandCreatedHandler(adsk.core.CommandCreatedEventHandler):
    def notify(self, eventArgs):
        cmd = eventArgs.command
        inputs = cmd.commandInputs

        s = loadSettings()
        inputs.addStringValueInput('exportPath', 'Export Path:', s['exportPath'])
        inputs.addBoolValueInput('mergeExisting', 'Keep Settings From Existing File:', True, '', s['mergeExisting'])
        inputs.addIntegerSpinnerCommandInput('turns', 'Turns (90° CCW):', 0, 3, 1, s['turns'])
        inputs.addFloatSpinnerCommandInput('mass', 'Mass (lb):', '', 0.0, 100.0, 0.5, s['mass'])
        gearset = inputs.addDropDownCommandInput('gearset', 'Default Gearset:', adsk.core.DropDownStyles.TextListDropDownStyle)
        gearsets = ['red', 'green', 'blue']
        selected = s['gearset'] if s['gearset'] in gearsets else DEFAULT_SETTINGS['gearset']
        for name in gearsets:
            gearset.listItems.add(name, name == selected)
        inputs.addFloatSpinnerCommandInput('motorRatio', 'Default Motor Ratio:', '', 0.01, 100.0, 0.25, s['motorRatio'])
        inputs.addStringValueInput('excludeRegex', 'Exclude Regex:', s['excludeRegex'])
        inputs.addBoolValueInput('previewExclusions', 'Hide Excluded Parts In Preview:', True, '', s['previewExclusions'])
        inputs.addStringValueInput('fullWattMotorRegex', '11w Motor Regex:', s['fullWattMotorRegex'])
        inputs.addStringValueInput('halfWattMotorRegex', '5.5w Motor Regex:', s['halfWattMotorRegex'])
        inputs.addStringValueInput('omniWheelRegex', 'Omni Wheel Regex:', s['omniWheelRegex'])
        inputs.addStringValueInput('tractionWheelRegex', 'Traction Wheel Regex:', s['tractionWheelRegex'])
        inputs.addStringValueInput('flexWheelRegex', 'Flex Wheel Regex:', s['flexWheelRegex'])
        inputs.addStringValueInput('imuRegex', 'IMU Regex:', s['imuRegex'])
        inputs.addStringValueInput('distanceRegex', 'Distance Regex:', s['distanceRegex'])
        inputs.addStringValueInput('rotationRegex', 'Rotation Regex:', s['rotationRegex'])
        inputs.addFloatSpinnerCommandInput('motorAngleOffset', 'Motor Angle Offset (deg):', '', -180.0, 180.0, 90.0, s['motorAngleOffset'])
        inputs.addFloatSpinnerCommandInput('wheelAngleOffset', 'Wheel Angle Offset (deg):', '', -180.0, 180.0, 90.0, s['wheelAngleOffset'])
        inputs.addBoolValueInput('caseSensitive', 'Case Sensitive:', True, '', s['caseSensitive'])

        onExecute = CommandExecuteHandler()
        cmd.execute.add(onExecute)
        handlers.append(onExecute)

        onPreview = CommandExecutePreviewHandler()
        cmd.executePreview.add(onPreview)
        handlers.append(onPreview)

        onDestroy = CommandDestroyHandler()
        cmd.destroy.add(onDestroy)
        handlers.append(onDestroy)

class CommandExecutePreviewHandler(adsk.core.CommandEventHandler):
    def notify(self, eventArgs):
        try:
            inputs = eventArgs.command.commandInputs
            if not inputs.itemById('previewExclusions').value:
                return
            excludeRegex = processRegex(inputs.itemById('excludeRegex').value,
                                        inputs.itemById('caseSensitive').value)
            design = adsk.fusion.Design.cast(app.activeProduct)
            if design is None:
                return

            def walk(occ):
                if not occ.isLightBulbOn:
                    return
                if excludeRegex.search(occ.name):
                    occ.isLightBulbOn = False
                    return
                for child in occ.childOccurrences:
                    walk(child)
            for occ in design.rootComponent.occurrences:
                walk(occ)
        except re.error:
            pass
        except: #pylint:disable=bare-except
            ui.messageBox(f'Error:\n{traceback.format_exc()}')

class CommandExecuteHandler(adsk.core.CommandEventHandler):
    def notify(self, eventArgs):
        try:
            inputs = eventArgs.command.commandInputs
            saveSettings(inputs)
            self.export(inputs)
        except ExportError as e:
            ui.messageBox(f'Export failed: {e}')
        except: #pylint:disable=bare-except
            ui.messageBox(f'Error:\n{traceback.format_exc()}')

    def export(self, inputs):
        value = lambda id: inputs.itemById(id).value
        exportPath = value('exportPath').strip()
        if not exportPath:
            raise ExportError('Enter an export path.')
        mass = value('mass')
        if mass == 0:
            raise ExportError('Enter the robot mass in pounds.')
        turns = value('turns')
        gearset = inputs.itemById('gearset').selectedItem.name
        motorRatio = value('motorRatio')
        motorAngleOffset = value('motorAngleOffset')
        wheelAngleOffset = value('wheelAngleOffset')
        caseSensitive = value('caseSensitive')
        regex = lambda id: processRegex(value(id), caseSensitive)
        excludeRegex = regex('excludeRegex')
        classifiers = [
            (regex('fullWattMotorRegex'), 'motor', 'full'),
            (regex('halfWattMotorRegex'), 'motor', 'half'),
            (regex('omniWheelRegex'), 'wheel', 'omni'),
            (regex('tractionWheelRegex'), 'wheel', 'traction'),
            (regex('flexWheelRegex'), 'wheel', 'flex'),
            (regex('imuRegex'), 'imu', None),
            (regex('distanceRegex'), 'distance', None),
            (regex('rotationRegex'), 'rotation', None),
        ]

        def isIncluded(occ):
            return occ.isLightBulbOn and excludeRegex.search(occ.name) is None

        design = adsk.fusion.Design.cast(app.activeProduct)
        if design is None:
            raise ExportError('The active product is not a Fusion design.')
        root = design.rootComponent

        bbox = getModelBbox(design, isIncluded)
        width = bbox.maxPoint.x - bbox.minPoint.x
        height = bbox.maxPoint.y - bbox.minPoint.y
        if turns % 2 == 1:
            width, height = height, width
        frame = Frame((bbox.minPoint.x + bbox.maxPoint.x) / 2,
                      (bbox.minPoint.y + bbox.maxPoint.y) / 2,
                      turns)

        def makeDevice(occ, deviceType, kind) -> dict:
            obb = Obb(occ, frame)
            x, y = round(obb.x, 4), round(obb.y, 4)
            if deviceType == 'motor':
                d = {'type': 'motor', 'port': None, 'id': 0, 'role': None, 'power': kind}
                if kind == 'full':
                    d['gearset'] = gearset
                d.update({'ratio': motorRatio, 'reversed': None, 'x': x, 'y': y,
                          'angle': normalizeAxisAngle(obb.longestHorizontalHeading() + motorAngleOffset)})
                return d
            if deviceType == 'wheel':
                axleX, axleY, _, _ = min(obb.axes, key=lambda a: a[3])
                extents = [a[3] for a in obb.axes]
                return {'type': 'wheel', 'subtype': kind, 'id': 0, 'x': x, 'y': y,
                        'angle': normalizeAxisAngle(heading(axleX, axleY) - 90 + wheelAngleOffset),
                        'width': round(min(extents), 4), 'diameter': round(max(extents), 4)}
            if deviceType == 'distance':
                d = {'type': 'distance', 'port': None, 'x': x, 'y': y, 'angle': None}
                axis = distanceSensorAxis(occ, obb, frame)
                app.log(f'  at ({x:.2f}, {y:.2f}); heavier end heading {heading(axis[0], axis[1]):.0f} deg, '
                        f'center of mass offset {axis[2]:.4f} in')
                distanceSensors.append((d, occ.component.name, axis))
                return d
            angle = normalizeAxisAngle(obb.longestHorizontalHeading())
            if deviceType == 'imu':
                return {'type': 'imu', 'port': None, 'x': x, 'y': y, 'angle': angle}
            return {'type': 'rotation', 'port': None, 'id': 0, 'ratio': 1.0, 'reversed': None,
                    'x': x, 'y': y, 'angle': angle}

        devices: list[dict] = []
        distanceSensors = []
        def walk(occ):
            if not occ.isLightBulbOn:
                app.log(f'Excluded (hidden): {occ.name}')
                return
            if not isIncluded(occ):
                app.log(f'Excluded (matches exclude regex): {occ.name}')
                return
            for pattern, deviceType, kind in classifiers:
                if pattern.search(occ.name):
                    app.log(f'{deviceType}: {occ.name}')
                    devices.append(makeDevice(occ, deviceType, kind))
                    return
            app.log(f'Searching: {occ.name} ({plural(occ.childOccurrences.count, "child occurrence", "child occurrences")}, '
                    f'{plural(occ.bRepBodies.count, "body", "bodies")})')
            for c in occ.childOccurrences:
                walk(c)
        for occ in root.occurrences:
            walk(occ)

        orientDistanceSensors(distanceSensors)
        assignIds(devices)

        mergeNote = ''
        if value('mergeExisting') and os.path.exists(exportPath):
            try:
                with open(exportPath) as file:
                    oldDevices = json.load(file).get('devices', [])
                matches = mergeExisting(devices, oldDevices)
                mergeNote = f'\nKept settings for {matches} of {plural(len(devices), "device", "devices")} from the existing file.'
            except (OSError, ValueError, AttributeError) as e:
                mergeNote = f'\nCould not read the existing file, settings not kept: {e}'

        assignPorts(devices)

        robotHeight = round(cmToInches(height), 4)
        start = {'x': round(FIELD_LEFT + robotHeight / 2, 4), 'y': 0.0, 'angle': 90.0}
        if os.path.exists(exportPath):
            try:
                with open(exportPath) as file:
                    old = json.load(file)
                start.update({k: old[k] for k in start if isinstance(old.get(k), (int, float))})
            except (OSError, ValueError, AttributeError):
                pass
        d = {'$schema': './robot.schema.json', 'version': 1,
             'width': round(cmToInches(width), 4), 'height': robotHeight,
             'mass': mass, **start, 'devices': devices}
        with open(exportPath, 'w') as file:
            json.dump(d, file, indent=4)
            file.write('\n')

        typeCounts = [(sum(dev['type'] == t for dev in devices), t) for t in DEVICE_NAMES]
        counts = joinList([plural(n, *DEVICE_NAMES[t]) for n, t in typeCounts if n > 0]) or 'no devices'
        nulls = sum(v is None for dev in devices for v in dev.values())
        nullNote = (f'\n{plural(nulls, "field is", "fields are")} null and must be filled in before running the sim.'
                    if nulls else '')
        ui.messageBox(f'Exported {counts} to {os.path.abspath(exportPath)}{mergeNote}{nullNote}')


class CommandDestroyHandler(adsk.core.CommandEventHandler):
    def notify(self, eventArgs):
        adsk.terminate()

def run(_context: str):
    try:
        cmdDefs = ui.commandDefinitions
        cmdDef = cmdDefs.itemById(COMMAND_ID)
        if cmdDef:
            cmdDef.deleteMe()
        cmdDef = cmdDefs.addButtonDefinition(COMMAND_ID, COMMAND_ID, '')

        onCreated = CommandCreatedHandler()
        cmdDef.commandCreated.add(onCreated)
        handlers.append(onCreated)

        cmdDef.execute()
        adsk.autoTerminate(False)
    except: #pylint:disable=bare-except
        ui.messageBox(f'Error:\n{traceback.format_exc()}')
