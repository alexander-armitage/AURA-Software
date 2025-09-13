<div align="center">

# AURA
### Autonomous Universal Robotics Architecture
This project is still under development
</div>

## Contents

1. [Overview](#overview)
2. [Installation](#installation)
3. [Instructions](#instructions)
4. [Repository Structure](#repository-structure)
5. [License](#license)

## Overview

## Installations

## Instructions

## Repository Structure
```
AURA-Software/
├── .github/                # Github actions
├── docs/                   # Schematics for the PCB projects
|
├── lib/                    # Libraries designed to be standalone
│ ├── Quat/                 # Orientation focused mathematics library (full madgwick implementation)
│ └── PID/                  # PID controller
|
├── src/                    # Source code directory
│ ├── common/               # Common libraries for the AURA-Hardware
| │ ├── bus_drivers/        # Drivers for bus protocols (I2C, SPI, UART)
| | ├── dev_drivers/        # Drivers for devices (SD, Flash, BMP280, ICM20948)
| | └── utility/            # Auxillary definitions (hardware pins, sidekick graphing macros)
| |
│ └── main/                 # Main code to run
| | ├── main_core_0/        # Code to run on the first core (EDIT THIS FILE)
| | | └── main_core_0.cpp
| | ├── main_core_1/        # Code to run on the second core (EDIT THIS FILE)
| | | └── main_core_1.cpp
| | └── main.cpp            # Main file that has app_main
|
├── tests/                  # Unit test folder
│ ├── googletest/           # Googletest submodule
│ └── main.cpp              # Test program (UNDER DEVELOPMENT)
|
├── .gitignore              # Git ignore file
├── README.md               # Project overview
└── LICENSE                 # License file
```

## License
See the [LICENSE](./LICENSE) file for more details.