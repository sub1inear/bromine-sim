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
- CMake
- Python (with `pip` or `uv`)
- GCC or Clang (MSVC is not supported)

1. Clone the repository.
2. Run `scripts/setup.py path/to/pros/project` with `pip`/`pip3`/`uv` to set up the repository with your PROS project.
3. Build it with CMake.
4. Run the resulting executable inside the root directory.

## Controls
BromineSim supports any gamepad or keyboard for input, including VEX V5 controllers.
| V5 | Nintendo | Xbox | PlayStation | Keyboard (TODO) |
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
| ` (TODO) | Toggle debug overlay |
| 1 (TODO) | Start freeplay (default) |
| 2 (TODO) | Start match |
| 3 (TODO) | Start driver skills |
| 4 (TODO) | Start auton skills |

The first controller detected will be the "master" controller, and the second controller will be the "partner" controller; any others will be ignored.
