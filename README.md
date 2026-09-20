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
- [`src/pros`] Modified PROS source code to run on Windows/POSIX
- [`src/sdk`] VEX SDK wrappers to connect PROS and the simulator
- [`src/sim`] Simulator code (graphics, physics, etc.)
- [`include/pros`] PROS headers
- [`include/sdk`] Documented VEX SDK headers
- [`include/sim`] Simulator headers
