# BromineSim

BromineSim is a Vex V5 Robot Simulator.

## Features

Planned features:
- PROS C++ emulation
- 2D graphics/physics simulation
- Basic sensor emulation (motors, distance sensors, inertial sensors, etc.)
- Customizable robot configuration (can auto-generate from Fusion)
- Robust warning system to catch errors early

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
- [`src/user`]: User source code
- [`include/pros`]: PROS headers
- [`include/sdk`]: Documented VEX SDK headers
- [`include/sim`]: Simulator headers
- [`include/user`]: User headers

[`src/pros`]: ./src/pros
[`src/sdk`]: ./src/sdk
[`src/sim`]: ./src/sim
[`src/user`]: ./src/user
[`include/pros`]: ./include/pros
[`include/sdk`]: ./include/sdk
[`include/sim`]: ./include/sim
[`include/user`]: ./src/user

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

The executable now lives in `build/` (specific location may vary depending on your system and CMake configuration).

`scripts/setup.py` symlinks your source code into the PROS project and fetches the source code for any libraries.
Redo this step if you install a new one.

## Controls
BromineSim supports any gamepad or keyboard for input, including VEX V5 controllers.
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

Keyboard controls:
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

Because user code cannot be restarted, BromineSim has no restart key. Instead, exit and relaunch the simulator.

The power key on the actual V5 is not available in the simulator; press `P` instead.

## Compatability
BromineSim works on Windows, macOS, and Linux (TODO).

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
