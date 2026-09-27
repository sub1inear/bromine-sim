# BromineSim

BromineSim is a Vex V5 Robot Simulator.

## Features

Planned features:
- PROS C++ emulation
- 2D graphics/physics simulation
- Basic sensor emulation (motors, distance sensors, inertial sensors, etc.)
- Customizable robot configuration (can auto-generate from Fusion)

Not planned:
- 3D physics simulation
- Full QEMU-based emulation
- Vexide Simulator Protocol support
- Full sensor support
- VEXCode support
- Auto-generation of robot configuration from other CAD softwares

## Architecture
- [`src/pros`]: Modified PROS source code to run on Windows/POSIX
- [`src/sdk`]: VEX SDK wrappers to connect PROS and the simulator
- [`src/sim`]: Simulator code (graphics, physics, etc.)
- [`include/pros`]: PROS headers
- [`include/sdk`]: Documented VEX SDK headers
- [`include/sim`]: Simulator headers

[`src/pros`]: ./src/pros
[`src/sdk`]: ./src/sdk
[`src/sim`]: ./src/sim
[`include/pros`]: ./include/pros
[`include/sdk`]: ./include/sdk
[`include/sim`]: ./include/sim

## Usage

Requirements:
- Git
- CMake
- Python
- UV
- GCC or Clang (MSVC is not supported)

```bash
git clone git@github.com:sub1inear/BromineSim.git
cd BromineSim
uv run scripts/setup.py path/to/pros/project
code assets/robot.json # Edit to match your robot configuration or export it from Fusion.
cmake -S . -B build
cmake --build build
```

## Controls
BromineSim supports any gamepad or keyboard for input, including VEX V5 controllers.
| V5 | Nintendo | Xbox | PlayStation | Keyboard |
|----|----------|------|-------------|----------|
| Left Stick | Left Stick | Left Stick | WASD |
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
| Power (⏻) | + / − | Menu / View | Options / Create | P |

Keyboard controls:
| Key | Action |
|-----|--------|
| Esc | Exit |
| ` or ~ | Toggle debug overlay |
| 1 (TODO) | Start freeplay (default) |
| 2 (TODO) | Start match |
| 3 (TODO) | Start driver skills |
| 4 (TODO) | Start auton skills |

The first controller detected will be the "master" controller, and the second controller will be the "partner" controller; any others will be ignored.

## Compatability
BromineSim works on Windows, macOS, and Linux (coming soon).
Most robot code should work without modification.
Many devices are intentionally unsupported.
The internal PROS layer is mostly unchanged, so most PROS functions should behave as expected.
Speed, undefined behavior, and physics will likely vary between the simulator and actual VEX hardware.
Some functions (like `.move_voltage()`) have intentionally undocumented behavior, so it is impossible to simulate them correctly.
To write simulator- or robot-specific code, conditionally compile based on the `BROMINE_SIM` macro.
