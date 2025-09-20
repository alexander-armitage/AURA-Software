<div align="center">

# AURA
### Autonomous Universal Robotics Architecture
![build_check](https://github.com/alexander-armitage/AURA-Software/actions/workflows/build_check.yaml/badge.svg)

This project is still under development
</div>

## Contents

1. [Overview](#overview)
2. [Installation](#installation)
3. [Instructions](#instructions)
4. [Repository Structure](#repository-structure)
5. [License](#license)

## Overview
AURA (Autonomous Universal Robotics Architecture) is a modular set of PCBs designed for robotics applications. This repository hosts the software designed using the [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/index.html). To make the PCBs easier to use, this software contains libraries to run all of the sensors and other peripherals. It is worth noting that despite the ESP-IDF being written in C, this is a C++ framework and makes heavy use of classes.

## Installation
This repository is a template repository designed to be used and filled in. To compile a project for the [AURA-Hardware](https://github.com/alexander-armitage/AURA-Hardware), the ESP-IDF must be installed following the instructions [here](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html#installation). Once setup, the target should be changed to `esp32s3` and the clock speed should be ideally set to `240MHz`.

## Instructions
### Setting up a project
After following the [installation](#installation), in the root directory `idf.py build` will compile the project or `idf.py flash` will flash the project to the target device.
### Using libraries
#### I2C
#### SPI
#### ICM20948
#### BMP280
#### INA219
#### PCA9685
#### Flash
#### SD Card
#### FSA8S
### Build process
The build process for this project uses CMake and is confifure in the root CMakeLists.txt. If a custom library is to be added, the library must be added to the root CMakeLists.txt and the directory which is added must also have a CMakeLists.txt in the form of:
```
idf_component_register(
  SRCS **SOURCE_FILE_CPP**
  INCLUDE_DIRS **DIRECTORIES**
  REQUIRES **REQUIREMENTS**
)
```

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