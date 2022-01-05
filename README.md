
[![Arduino CI](https://github.com/RobTillaart/AM2315/workflows/Arduino%20CI/badge.svg)](https://github.com/marketplace/actions/arduino_ci)
[![Arduino-lint](https://github.com/RobTillaart/AM2315/actions/workflows/arduino-lint.yml/badge.svg)](https://github.com/RobTillaart/AM2315/actions/workflows/arduino-lint.yml)
[![JSON check](https://github.com/RobTillaart/AM2315/actions/workflows/jsoncheck.yml/badge.svg)](https://github.com/RobTillaart/AM2315/actions/workflows/jsoncheck.yml)
[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](https://github.com/RobTillaart/AM2315/blob/master/LICENSE)
[![GitHub release](https://img.shields.io/github/release/RobTillaart/AM2315.svg?maxAge=3600)](https://github.com/RobTillaart/AM2315/releases)


# AM2315

Arduino library for I2C AM2315 temperature and humidity sensor.


## Description

**Experimental**

The library should be initiated by calling the **begin()** function, 
optionally **begin(dataPin, clockPin)** for **ESP32** and similar platforms.

Thereafter one has to call the **read()** function to do the actual reading,
and with **getTemperature()** and **getHumidity()** to get the read values.
Calling these latter again will return the same values until a new **read()** is called.


## Interface


### Constructor

- **AM2315(TwoWire \*wire)** constructor, using a specific Wire (I2C bus).
- **bool begin(uint8_t dataPin, uint8_t clockPin)** begin for ESP32 et al, to set I2C bus pins
- **bool begin()** initializer for non ESP32. Returns true if connected.
- **bool isConnected()** returns true if the address of the AM2315 can be seen on the I2C bus.


### Core

- **int8_t read()** read the sensor and store the values internally. It returns the status of the read which should be 0.
- **float getHumidity()** returns last Humidity read, or -999 in case of error.
- **float getTemperature()** returns last Temperature read, or -999 in case of error.
- **uint32_t lastRead()** returns the timestamp in milliseconds since startup of the last successful read.


### Offset

- **void setHumOffset(float offset)** set an offset to calibrate (1st order) the sensor.
- **float getHumOffset()** return current offset, default 0.
- **void setTempOffset(float offset)** set an offset to calibrate (1st order) the sensor
- **float getTempOffset()** return current offset, default 0.


## Operation

See examples


## Future

- get hardware and test
- documentation


