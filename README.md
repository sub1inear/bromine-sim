# bromine-sim
`bromine-sim` is a Vex V5 Robot Simulator.

## Features
Planned features:
- PROS C++ emulation
- LVGL/LCD screen emulation
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


## Usage

Requirements:

- [Git](https://git-scm.com/)
- [CMake](https://cmake.org/)
- [Python](https://www.python.org/)
- [uv](https://docs.astral.sh/uv/)
- [GCC](https://gcc.gnu.org/) or [Clang](https://clang.llvm.org/) (MSVC is not supported)
- [PROS](https://pros.cs.purdue.edu/)

Instructions:
1. Create a PROS project if you don't already have one.

    VSCode: Install the PROS extension and go `Conductor > Create Project`.

    CLI: Run `pros c new-project path/to/pros/project`.
1. Clone the repository and enter it.
    ```bash
    git clone git@github.com:sub1inear/bromine-sim.git
    cd bromine-sim
    ```
2. Link your PROS project and its libraries (redo if any added).
    ```bash
    sudo uv run scripts/setup.py path/to/pros/project
    ```
3. Edit `assets/robot.json` to match your robot, or export it from Fusion (TODO).
    ```bash
    code assets/robot.json
    ```
4. Build the project with CMake and run the simulator.
    ```bash
    cmake -S . -B build
    cmake --build build
    ```
    Windows: `build\bromine-sim.exe`

    Linux/macOS: `build/bromine-sim`
## Controls
`bromine-sim` supports any gamepad or keyboard for input, including VEX V5 controllers.
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

Because user code cannot be restarted, bromine-sim has no restart key. Instead, exit and relaunch the simulator.

The power key on the actual V5 is not available in the simulator; press `P` instead.

## Compatability
`bromine-sim` works on Windows, macOS, and Linux.

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
[`include/user`]: ./include/user
