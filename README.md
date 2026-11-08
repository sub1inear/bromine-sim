# Bromine Sim
Bromine Sim is a Vex V5 Robot Simulator.

## Features
### Implemented
- PROS C++ emulation
- VSCode extension
- LVGL/LCD screen emulation
- 2D graphics/physics simulation
- Basic sensor emulation (motors, distance sensors, inertial sensors, etc.)
- Customizable robot configuration (can auto-generate from Fusion)
- Warning system to catch issues early

### Not Planned
- 3D physics simulation
- Full QEMU-based emulation
- Vexide Simulator Protocol support
- Full sensor support
- VEXCode support
- Auto-generation of robot configuration from other CAD softwares


## Usage

1. Install the [Bromine Sim](https://marketplace.visualstudio.com/items?itemName=sublinear.bromine-sim) extension.
2. Open up a PROS project.
3. Create a `robot.json` with the Fusion exporter or by hand.
   1. Open Fusion.
   2. Go to `Utilities → Add-Ins → Scripts and Add-Ins`.
   3. Click the `+` next to `My Scripts`.
   4. Select the [`scripts/bromine-sim-fusion-export`](scripts/bromine-sim-fusion-export) folder.
   5. Click `Run` on the script.
   6. Input the configuration details.
   7. Run. It may take some time to finish.
4. Click `Run Sim` on the top right to run your code with the simulator.
5. Click `Debug Sim` on the top right to debug your with the simulator.

[`scripts/bromine-sim-fusion-export`]: ./scripts/bromine-sim-fusion-export

## Controls
Bromine Sim supports any gamepad or keyboard for input, including VEX V5 controllers.

### Robot Controls
| V5 | Nintendo | Xbox | PlayStation | Keyboard |
|----|----------|------|-------------|----------|
| Left Stick | Left Stick | Left Stick | Left Stick | WASD |
| Right Stick | Right Stick | Right Stick | Right Stick | IJKL |
| D-Pad | D-Pad | D-Pad | D-Pad | Arrow Keys |
| L1 | L | LB | L1 | Q |
| L2 | ZL | LT | L2 | E |
| R1 | R | RB | R1 | U |
| R2 | ZR | RT | R2 | O |
| A (right) | A | B | Circle | Z |
| B (bottom) | B | A | Cross | X |
| X (top) | X | Y | Triangle | C |
| Y (left) | Y | X | Square | V |
| Power  | + / − | Menu / View | Options / Create | P |

### Simulator Controls
| Key | Action |
|-----|--------|
| Esc | Exit |
| ` or ~ | Toggle debug overlay |
| 1 | Practice (default) |
| 2 | Match |
| 3 | Driver skills |
| 4 | Auton skills |
| Space | Start |

The first controller detected will be the "master" controller, and the second controller will be the "partner" controller; any others will be ignored.

Because user code cannot be restarted, Bromine Sim has no restart key. Instead, exit and relaunch the simulator.

The power key on the actual V5 is not available in the simulator; press `P` instead.

## Compatability
Bromine Sim works on Windows, macOS, and Linux.

### Strengths
- The internal PROS layer is left mostly unchanged.
- The most common devices (motors, distance sensors, IMUs, etc.) are/will be supported.

### Limitations
- FreeRTOS is run with a PC port, so threading may behave differently between the simulator and the actual VEX hardware.
- Some devices are intentionally unsupported.
- Unportable C/C++ code (e.g. that assumes integer sizes or has inline assembly) may not work correctly.
- Performance may differ from actual VEX hardware.
- Some VEX functions (like `.move_velocity()`) have intentionally undocumented behavior, so it is impossible to simulate them correctly.
- Relying on any internal PROS headers like the hot tables may break.

To write simulator- or robot-specific code, conditionally compile based on the `BROMINE_SIM` macro.
Many of these limitations are rare in practice.

## Architecture
- [`src/pros`]: Modified PROS source code to run on Windows/POSIX
- [`src/sdk`]: VEX SDK wrappers to connect PROS and the simulator
- [`src/sim`]: Simulator code (graphics, physics, etc.)
- [`include/pros`]: PROS headers
- [`include/sdk`]: Documented VEX SDK headers
- [`include/sim`]: Simulator headers
- [`extension`]: Extension source code

[`src/pros`]: ./src/pros
[`src/sdk`]: ./src/sdk
[`src/sim`]: ./src/sim
[`src/user`]: ./src/user
[`include/pros`]: ./include/pros
[`include/sdk`]: ./include/sdk
[`include/sim`]: ./include/sim
[`include/user`]: ./include/user
[`extension`]: ./extension

## License
See [LICENSE](./LICENSE) for details.
