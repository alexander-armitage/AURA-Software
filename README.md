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
   - [Setting up a Project](#setting-up-a-project)  
   - [Using Libraries](#using-libraries)  
     - [I2C](#i2c)  
     - [SPI](#spi)  
     - [ICM20948](#icm20948)
     - [BMP280](#bmp280)
     - [INA219](#ina219)
     - [PCA9685](#pca9685)
     - [Flash](#flash)
     - [SD Card](#sd-card)
   - [Build process](#build-process)
4. [Contributing](#contributing)
   - [Contributing Code](#contributing-code)
   - [Issues](#issues)
5. [Repository Structure](#repository-structure)
6. [License](#license)

## Overview
AURA (Autonomous Universal Robotics Architecture) is a modular set of PCBs designed for robotics applications. This repository hosts the software designed using the [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/index.html). To make the [AURA-Hardware PCBs](https://github.com/alexander-armitage/AURA-Hardware) easier to use, this software contains libraries to run all of the sensors and other peripherals. It is worth noting that despite the ESP-IDF being written in C, this is a C++ framework and makes heavy use of classes.

A good demonstrator/example project of how to use this framework is the [AURA-Quadcopter](https://github.com/alexander-armitage/AURA-Quadcopter) repository.

## Installation
This repository is a template repository designed to be used and filled in. To compile a project for the [AURA-Hardware](https://github.com/alexander-armitage/AURA-Hardware), the ESP-IDF must be installed following the instructions [here](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html#installation). Once setup, the target should be changed to `esp32s3` and the clock speed should be ideally set to `240MHz`.

## Instructions
### Setting up a project
After following the [installation](#installation), in the root directory `idf.py build` will compile the project or `idf.py flash` will flash the project to the target device.
### Using libraries
#### I2C
This library wraps the ESP-IDF I2C functions in an easy-to-use class which gets passed in by reference to the sensor functions that use I2C. `i2c::i2c_bus my_bus(SDA, SCL);` creates a bus object where `SDA` and `SCL` are of the `gpio_num_t` type.

The ESP-IDF `i2c_master_probe` function has also been wrapped and can be called through: `esp_err_t master_probe(uint8_t dev_addr)`.

#### SPI
This library is simply a wrapper of the ESP-IDF `spi_bus_initialize()` function. To initialise an SPI bus, an ESP-IDF SPI host of `spi_host_device_t` type must be defined (often `SPI2_HOST`). Then `spi::create_spi_bus(SPI_HOST, MOSI, MISO, SCLK);` initialises the SPI bus.

#### ICM20948
Once an I2C bus has been [initialised](#i2c), `icm20948::icm20948 my_icm(&my_bus);` creates an icm20948 object. Within this class there are essentially two I2C devices: the magnetometer, and the gyroscope and accelerometer which are on one bus.

To read the gyroscope and accelerometer values first run `my_icm.update()` which will read the gyroscope and accelerometer code in one go to save time. After updating, the values (m/s^2 for accelerometer and rad/s for gyroscope) can be accested through the getter functions:
- `float my_icm.acc_x()`
- `float my_icm.acc_y()`
- `float my_icm.acc_z()`
- `float my_icm.gyro_x()`
- `float my_icm.gyro_y()`
- `float my_icm.gyro_z()`

To read the magnetometer values first run `my_icm.update_mag()` which will read the magnetometer values in a specific sequence described in the [AK09916 datasheet](https://www.y-ic.es/datasheet/78/SMDSW.020-2OZ.pdf). After updating, the values (uT) can be accested through the getter functions:
- `float my_icm.mag_x()`
- `float my_icm.mag_y()`
- `float my_icm.mag_z()`

#### BMP280
Once an I2C bus has been [initialised](#i2c), `bmp280::bmp280 my_bmp(&my_bus);` creates a bmp280 object. `my_bmp.update()` updates the pressure and temperature readings which can then be accessed through: `float my_bmp.get_pressure()`, `float my_bmp.get_temperature()` which return kPa and degrees Celsius respectively. To use the BMP280 for altitude calculation, first call `my_bmp.set_height(float current_height)` which will set the current height otherwise the initial height is assumed to be 0. Then, `float my_bmp.get_altitude()` returns the altitude in m.

#### INA219
Once an I2C bus has been [initialised](#i2c), `ina219::ina219 my_ina(&my_bus);` creates a ina219 object. The voltage (V), current (A), and power (W) can be accessed using `float get_voltage()`, `float get_current()`, `float get_power()`, respectively. Unlike the other I2C libraries, each function reads the respective register and converts to the correct units.

It is worth noting this library is currently very statically coded for the [AURA-Hardware](https://github.com/alexander-armitage/AURA-Hardware) so the shunt resistor is 0m5 Ohms, which gives a current sensing range of `640A` although this is limited to `75A` constinuously due to the power rating of the resistor, with a resolution of `19.5mA` and `320mV`.

#### PCA9685
Once an I2C bus has been [initialised](#i2c), `pca9685::pca9685 my_pca(&my_bus, uint16_t freq, uint8_t addr);` creates a pca9685 object where `freq` is 50Hz by default and `addr` is `0x40`.

The base class `pca9685` has two functions: `esp_err_t set_pwm(uint8_t ch_num, uint16_t pulse)` and `esp_err_t set_pwm_all(uint16_t pulse)`, where `0 <= pulse <= 4096` and `0 <= ch_num <= 15`.

To make motor control easier, there is a `pca_esc` class which is initialised using `pca9685::pca_esc(&my_pca, uint8_t channel)`. There is a `esp_err_t set_throttle(uint8_t percent)` function to set the motor speed to a percent `0-100` inclusive. For safety, there is a `set_armed(bool armed)` function to set the motor as armed or not, if not armed, `set_throttle()` will not spin the motor no matter the `percent` input. To check the state: `bool armed()`.

#### Flash
**WARNING: When using the SD Card and Flash, both files cannot be accessed simultaneously and the file MUST be closed properly using `fclose(FILE*)`**

Once an SPI bus has been [initialised](#spi), `flash::flash my_flash(gpio_num_t cs, spi_host_device_t spi_bus);` will initialise a flash object.

To mount a FAT file system partition there are two options: `my_flash.force_mount_fat_partition()`, or `my_flash.mount_fat_partition()`. `my_flash.force_mount_fat_partition()` will erase any data from the flash and must be called when setting up a flash chip for the first time. For normal use, use `my_flash.mount_fat_partition()`.

To list all od the data partitions on the flash chip: `my_flash.list_data_partitions()` will print every partition to the terminal.

The flash can then be used like any other directory labelled as `"/ext_flash"` which can be accessed via `flash::FLASH_DIR`.

Once finished or no longer needed, `my_flash.unmount_fat_partition()` will remove the FAT file system.

#### SD Card
**WARNING: When using the SD Card and Flash, both files cannot be accessed simultaneously and the file MUST be closed properly using `fclose(FILE*)`**

Once an SPI bus has been [initialised](#spi), `sd_card::sd_card my_sd_card(gpio_num_t cs, spi_host_device_t spi_bus);` will initialise a sd_card object. `mount_sd_card(spi_host_device_t spi_bus)` will mount a FAT file system onto the SD card so it can be accessed like any directory `"/sd_card"`. This directory is defined as `sd_card::SD_DIR`. Use `unmount_sd_card()` to unmount the SD card.

### Build process
The build process for this project uses CMake and is configured in the root CMakeLists.txt. If a custom library is to be added, the library must be added to the root CMakeLists.txt and the directory which is added must also have a CMakeLists.txt in the form of:
```
idf_component_register(
  SRCS **SOURCE_FILE_CPP**
  INCLUDE_DIRS **DIRECTORIES**
  REQUIRES **REQUIREMENTS**
)
```
To add current libraries to files you must also include them in the CMakeLists.txt of the file which is including them.

## Contributing
### Contributing Code
This project is still early in development and has current only had a month and a half of development, so any contribution is welcome. To contribute, first fork the repository and use it with any esp dev kit and breakout modules. All code is written roughly following the Google style guide (clang) and should be thoroughly commented. Once the feature or fix has been made and bug tested, create a pull request which will be merged if it contributed positively to the framework. Current ideas would be: a kalman filter, LQR control, better I2C reading and writing functions.

### Issues
As this framework is still in early development, issues are expected. Please raise them on GitHub with the hardware schematic (if applicable), and code causing the issue.

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