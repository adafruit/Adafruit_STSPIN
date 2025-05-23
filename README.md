# Adafruit STSPIN Library [![Build Status](https://github.com/adafruit/Adafruit_STSPIN/workflows/Arduino%20Library%20CI/badge.svg)](https://github.com/adafruit/Adafruit_STSPIN/actions)[![Documentation](https://github.com/adafruit/ci-arduino/blob/master/assets/doxygen_badge.svg)](http://adafruit.github.io/Adafruit_STSPIN/html/index.html)

This is an Arduino library for the Adafruit STSPIN220 breakout board, a low voltage stepper motor driver.

## About the STSPIN220

The [STSPIN220](https://www.adafruit.com/products/XXXX) is a low voltage stepper motor driver that integrates both control logic and a low RDS(on) power stage in a small VFQFPN 3x3x1.0 mm package.

Key features:
* Operating voltage: 1.8V to 10V
* Maximum output current: 1.3A RMS
* Microstepping up to 1/256th of a step
* Ultra-low standby current (<80 nA)
* Complete protection set (overcurrent, thermal, short-circuit)
* Arduino Stepper library compatible interface
* Digital pin interface

## Installation

You can install this library through the Arduino Library Manager. Search for "Adafruit STSPIN" and install the latest version.

To manually install:
1. Download the .zip file from GitHub
2. In the Arduino IDE: Sketch -> Include Library -> Add .ZIP Library

## Hardware

* [Adafruit STSPIN220 - Low Voltage Stepper Motor Driver Breakout](https://www.adafruit.com/products/XXXX)

## Examples

The library includes examples demonstrating various features:

* **STSPIN220_Basic**: Basic usage with Arduino Stepper library compatible interface

## Library Features

* Initialize with minimal 2-pin or full 6-pin control
* Arduino Stepper library compatible methods (setSpeed, step, version)
* Configure microstepping resolution from full-step to 1/256 step
* Power management with enable/disable and ultra-low power standby
* Fault detection and clearing
* Single step and blocking step methods
* Automatic mode pin handling for breakout boards with pull-ups

## Documentation

For more information on using this library, check out the [examples](/examples) folder.

Full documentation of the STSPIN220 sensor can be found in the [datasheet](https://www.st.com/resource/en/datasheet/stspin220.pdf).

## License

This library is released under a BSD license. See the included LICENSE file for details.