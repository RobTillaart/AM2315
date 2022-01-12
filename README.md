
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

- **AM2315(TwoWire \*wire = &Wire)** constructor, default using Wire (I2C bus), optionally set to Wire0 .. WireN.
- **bool begin(uint8_t dataPin, uint8_t clockPin)** begin for ESP32 et al, to set I2C bus pins, returns true if device is connected.
- **bool begin()** initializer for non ESP32. Returns true if connected.
- **bool isConnected(uint16_t timeout = 3000)** returns true if the address of the AM2315 can be seen on the I2C bus.
As the device can be in sleep modus it will retry for the defined timeout (in micros) with a minimum of 1 try. 
minimum = 800 us and maximum = 3000 us according to datasheet.


### Core

- **int8_t read()** read the sensor and store the values internally. 
It returns the status of the read which should be **AM2315_OK** == 0.
- **float getHumidity()** returns last Humidity read, or -999 in case of error.
- **float getTemperature()** returns last Temperature read, or **AM2315_INVALID_VALUE** == -999 in case of error.
- **uint32_t lastRead()** returns the timestamp in milliseconds since startup of the last successful read.


### Offset

- **void setHumOffset(float offset)** set an offset to calibrate (1st order) the sensor.
- **float getHumOffset()** return current offset, default 0.
- **void setTempOffset(float offset)** set an offset to calibrate (1st order) the sensor
- **float getTempOffset()** return current offset, default 0.


### Control

Functions to adjust the communication with the sensor.

- **void setWaitForReading(bool b )** flag to enforce a blocking wait. 
- **bool getWaitForReading()** returns the above setting.
- **void setReadDelay(uint16_t rd = 0)** To tune the time it waits before actual read. Default = 2000 ms.
set readDelay to 0 will reset to 2000 ms AFTER a call to **read()**.
- **uint16_t getReadDelay()** returns the above setting.
- **void setSuppressError(bool b)** suppress error values of -999 => you need to check the return value of read() instead.  
This is used to keep spikes out of your graphs / logs. 
- **bool getSuppressError()**  returns the above setting.


### Misc

- **bool wakeUp()** function that will try for 3 milliseconds to wake up the sensor.


### error codes


| name                              | value | notes     |
|:----------------------------------|------:|:----------|
| AM2315_OK                         |  0    |
| AM2315_ERROR_CHECKSUM             |  -10  |
| AM2315_ERROR_CONNECT              |  -11  |
| AM2315_MISSING_BYTES              |  -12  |
| AM2315_WAITING_FOR_READ           |  -50  |
| AM2315_HUMIDITY_OUT_OF_RANGE      |  -100 |
| AM2315_TEMPERATURE_OUT_OF_RANGE   |  -101 |
| AM2315_INVALID_VALUE              |  -999 |


## Operation

See examples


## Future

- found that the interface is like AM232X library.need to test first.
- get hardware and test test test ...
- update unit test
- documentation
- clean up code
- add examples
- add AM2320 derived class ?
- optimize

**wont**
- add calls for meta information (no description yet)
  - 0x07 status register
  - 0x08-0x0B user register HIGH LOW HIGH2 LOW2

