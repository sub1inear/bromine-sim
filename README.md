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
- Python
- GCC or Clang (MSVC is not supported)

1. Clone the repository.
2. Run `scripts/setup.py path/to/pros/project` with Python (`python` on Windows, `python3` on Linux/MacOS) to set up everything.
3. Build it with CMake.
4. Run the resulting executable inside the root directory.

