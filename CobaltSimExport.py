"""This file acts as the main module for this script."""
# pyright: reportAttributeAccessIssue=false
import traceback
import adsk.core
import adsk.fusion
import json
import os
import math
import re

# import adsk.cam
# import adsk.drawing

app      = adsk.core.Application.get()
ui       = app.userInterface
handlers = []

def cmToInches(cm: int | float) -> int | float:
    return cm * 0.393701

def radToDeg(rad: float) -> float:
    return (rad * 180.0 / math.pi) % 360

def getWorldTransform(occ):
    m = occ.transform2.copy()
    ctx = occ.assemblyContext
    while ctx:
        m.transformBy(ctx.transform2)
        ctx = ctx.assemblyContext
    return m

def getModelBbox(design, excludeRegex) -> adsk.core.BoundingBox3D:
    root = design.rootComponent
    bbox = None

    def add(body):
        nonlocal bbox
        bb = body.boundingBox
        if bbox is None:
            bbox = adsk.core.BoundingBox3D.create(bb.minPoint, bb.maxPoint)
        else:
            bbox.combine(bb)

    def walk(occ):
        if excludeRegex.search(occ.name) is not None:
            return
        if not occ.isLightBulbOn:
            return
        for body in occ.bRepBodies:
            if body.isVisible:
                add(body)
        for child in occ.childOccurrences:
            walk(child)

    for body in root.bRepBodies:
        if body.isVisible:
            add(body)
    for occ in root.occurrences:
        walk(occ)

    return bbox # pyright: ignore[reportReturnType]

def rotatePointAroundOrigin(point: adsk.core.Point3D, angle: float) -> adsk.core.Point3D:
    axis = adsk.core.Vector3D.create(0, 0, 1)
    origin = adsk.core.Point3D.create(0, 0, 0)
    matrix = adsk.core.Matrix3D.create()
    matrix.setToRotation(angle, axis, origin)

    rotated = point.copy()
    rotated.transformBy(matrix)
    return rotated

def getCenterAndAngle(occ, angle):
    obb = occ.orientedMinimumBoundingBox

    m = adsk.core.Matrix3D.create()
    m.setToRotation(angle,
                    adsk.core.Vector3D.create(0, 0, 1),
                    adsk.core.Point3D.create(0, 0, 0))

    center = obb.centerPoint.copy()
    center.transformBy(m)

    d = obb.lengthDirection.copy()
    d.transformBy(m)
    boxAngle = math.atan2(d.x, d.y) % (2.0 * math.pi)
    return center, boxAngle

def processRegex(regex: str, caseSensitive: bool):
    # Make '' mean no match.
    processedRegex = '(?!)' if regex == '' else regex
    flags = re.NOFLAG if caseSensitive else re.IGNORECASE
    return re.compile(processedRegex, flags)

def combineRegexes(args: list[re.Pattern], caseSensitive):
    combinedRegexes = '|'.join([f'(?:{regex.pattern})' for regex in args])
    flags = re.NOFLAG if caseSensitive else re.IGNORECASE
    return re.compile(combinedRegexes, flags)

class CommandCreatedHandler(adsk.core.CommandCreatedEventHandler):
    def notify(self, eventArgs):
        cmd = eventArgs.command
        inputs = cmd.commandInputs

        inputs.addStringValueInput('exportPath', 'Export Path:', r'C:\Source\Vex\CobaltSim\assets\robot.json')
        inputs.addIntegerSpinnerCommandInput('turns', 'Turns:', 0, 3, 1, 1)
        inputs.addStringValueInput('excludeRegex', 'Exclude Regex:', 'blue')
        inputs.addStringValueInput('fullWattMotorRegex', '11w Motor Regex:', '11w')
        inputs.addStringValueInput('halfWattMotorRegex', '5.5w Motor Regex:', '5\\.5w')
        inputs.addStringValueInput('omniWheelRegex', 'Omni Wheel Regex:', '(?:omni)|(?:_2\\.762f066b-c76f-4c67-8be2-692f671d8685_LV0)')
        inputs.addStringValueInput('tractionWheelRegex', 'Traction Wheel Regex:', '')
        inputs.addStringValueInput('flexWheelRegex', 'Flex Wheel Regex:', '')
        inputs.addFloatSpinnerCommandInput('motorAngleOffset', 'Motor Angle Offset:','deg', 0.0, 359.0, 1.0, 0.0)
        inputs.addFloatSpinnerCommandInput('wheelAngleOffset', 'Wheel Angle Offset:', 'deg', 0.0, 359.0, 1.0, 90.0)
        inputs.addBoolValueInput('caseSensitive', 'Case Sensitive:', True, '', False)

        onExecute = CommandExecuteHandler()
        cmd.execute.add(onExecute)
        handlers.append(onExecute)

        onDestroy = CommandDestroyHandler()
        cmd.destroy.add(onDestroy)
        handlers.append(onDestroy)

class CommandExecuteHandler(adsk.core.CommandEventHandler):
    def notify(self, eventArgs):
        try:
            app.log("TextCommandWindow.Clear")
            inputs = eventArgs.command.commandInputs
            exportPath = inputs.itemById('exportPath').value
            turns = inputs.itemById('turns').value
            caseSensitive = inputs.itemById('caseSensitive').value
            excludeRegex = processRegex(inputs.itemById('excludeRegex').value, caseSensitive)
            fullWattMotorRegex = processRegex(inputs.itemById('fullWattMotorRegex').value, caseSensitive)
            halfWattMotorRegex = processRegex(inputs.itemById('halfWattMotorRegex').value, caseSensitive)
            omniWheelRegex = processRegex(inputs.itemById('omniWheelRegex').value, caseSensitive)
            tractionWheelRegex = processRegex(inputs.itemById('tractionWheelRegex').value, caseSensitive)
            flexWheelRegex = processRegex(inputs.itemById('flexWheelRegex').value, caseSensitive)

            motorAngleOffset = inputs.itemById('motorAngleOffset').value
            wheelAngleOffset = inputs.itemById('wheelAngleOffset').value

            motorRegex = combineRegexes([fullWattMotorRegex, halfWattMotorRegex], caseSensitive)
            wheelRegex = combineRegexes([omniWheelRegex, tractionWheelRegex, flexWheelRegex], caseSensitive)
            deviceRegex = combineRegexes([motorRegex, wheelRegex], caseSensitive)

            angle = turns * math.pi / 2

            design = adsk.fusion.Design.cast(app.activeProduct)
            root = design.rootComponent

            bbox = getModelBbox(design, excludeRegex)
            width = bbox.maxPoint.x - bbox.minPoint.x
            height = bbox.maxPoint.y - bbox.minPoint.y

            if turns == 1 or turns == 3:
                width, height = height, width

            d: dict = {'version': 1, 'width': cmToInches(width), 'height': cmToInches(height), 'mass': 18.0, 'devices': []}
            def walk(occ):
                app.log(f'{occ.name}')
                if deviceRegex.search(occ.name):
                    occCenter, occAngle = getCenterAndAngle(occ, angle)
                    device: dict[str, int | float | str] = {'x': cmToInches(occCenter.x),
                              'y': cmToInches(occCenter.y)}
                    if motorRegex.search(occ.name):
                        app.log(f'Motor: {occ.name}')
                        power = None
                        if fullWattMotorRegex.search(occ.name):
                            power = 'full'
                        elif halfWattMotorRegex.search(occ.name):
                            power = 'half'
                        else:
                            app.log(f'Error: Could not find motor power in {occ.name}')
                            return

                        device['type'] = 'motor'
                        device['port'] = 1
                        device['id'] = 1
                        device['gear'] = 'blue'
                        device['role'] = 'drive'
                        device['power'] = power
                        device['angle'] = radToDeg(occAngle + motorAngleOffset)
                    elif wheelRegex.search(occ.name):
                        app.log(f'Wheel: {occ.name}')
                        width = 2.75
                        diameter = 1.225

                        device['type'] = 'wheel'
                        device['id'] = 1
                        device['width'] = width
                        device['diameter'] = diameter
                        device['subtype'] = 'omni'
                        device['rpm'] = 400
                        device['angle'] = radToDeg(occAngle + wheelAngleOffset)
                    d['devices'].append(device)
                    return

                for c in occ.childOccurrences:
                    walk(c)
            for occ in root.occurrences:
                walk(occ)

            with open(exportPath, 'w') as file:
                json.dump(d, file, indent=4)

            ui.messageBox(f'Successfully exported to file {os.path.abspath(exportPath)}')
        except: #pylint:disable=bare-except
            ui.messageBox(f'Error:\n{traceback.format_exc()}')


class CommandDestroyHandler(adsk.core.CommandEventHandler):
    def notify(self, eventArgs):
        adsk.terminate()

def run(_context: str):
    try:
        cmdDefs = ui.commandDefinitions
        cmdDef = cmdDefs.itemById('Cobalt Sim Export')
        if cmdDef:
            cmdDef.deleteMe()
        cmdDef = cmdDefs.addButtonDefinition('Cobalt Sim Export', 'Cobalt Sim Export', '')

        onCreated = CommandCreatedHandler()
        cmdDef.commandCreated.add(onCreated)
        handlers.append(onCreated)

        cmdDef.execute()
        adsk.autoTerminate(False)
    except: #pylint:disable=bare-except
        ui.messageBox(f'Error:\n{traceback.format_exc()}')
