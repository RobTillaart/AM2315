//
//    FILE: AM2315.cpp
//  AUTHOR: Rob.Tillaart@gmail.com
// VERSION: 0.1.0
// PURPOSE: AM2315 Temperature and Humidity sensor library for Arduino
//     URL: https://github.com/RobTillaart/AM2315
//
//  HISTORY:
//  0.1.0  2022-01-05  initial version


#include "AM2315.h"


// these defines are not for user to adjust
// READ_DELAY for blocking read
#define AM2315_READ_DELAY           2000

#define AM2315_ADDRESS                        0x5C



/////////////////////////////////////////////////////
//
// PUBLIC
//
AM2315::AM2315(TwoWire *wire)
{
  _wire        = wire;
  _temperature = 0;
  _humidity    = 0;
  _humOffset   = 0;
  _tempOffset  = 0;
};


#if defined(ESP8266) || defined(ESP32)
bool AM2315::begin(const uint8_t dataPin, const uint8_t clockPin)
{
  if ((dataPin < 255) && (clockPin < 255))
  {
    _wire->begin(dataPin, clockPin);
  } else {
    _wire->begin();
  }
  return isConnected();
}
#endif


bool AM2315::begin()
{
  _wire->begin();
  return isConnected();
}


bool AM2315::isConnected()
{
  _wire->beginTransmission(AM2315_ADDRESS);
  int rv = _wire->endTransmission();
  return rv == 0;
}


int AM2315::read()
{
  // reset readDelay
  if (_readDelay == 0) _readDelay = 2000;
  while (millis() - _lastRead < _readDelay)
  {
    if (!_waitForRead) return AM2315_WAITING_FOR_READ;
    yield();
  }
  int rv = _read();
  return rv;
}


///////////////////////////////////////////////////////////
//
//  PRIVATE
//
int AM2315::_read()
{
  // READ VALUES
  int rv = _readSensor();
  interrupts();

  _lastRead = millis();

  if (rv != AM2315_OK)
  {
    if (_suppressError == false)
    {
      _humidity    = AM2315_INVALID_VALUE;
      _temperature = AM2315_INVALID_VALUE;
    }
    return rv;  // propagate error value
  }

  _humidity = (_bits[0] * 256 + _bits[1]) * 0.1;
  int16_t t = ((_bits[2] & 0x7F) * 256 + _bits[3]);
  if (t == 0)
  {
    _temperature = 0.0;     // prevent -0.0;
  }
  else
  {
    _temperature = t * 0.1;
    if ((_bits[2] & 0x80) == 0x80 )
    {
      _temperature = -_temperature;
    }
  }


  // TEST OUT OF RANGE
#ifdef AM2315_VALUE_OUT_OF_RANGE
  if (_humidity > 100)
  {
    return AM2315_HUMIDITY_OUT_OF_RANGE;
  }
  if ((_temperature < -40) || (_temperature > 80))
  {
    return AM2315_TEMPERATURE_OUT_OF_RANGE;
  }
#endif

  if (_humOffset != 0.0)
  {
    _humidity += _humOffset;
    if (_humidity < 0) _humidity = 0;
    if (_humidity > 100) _humidity = 100;
  }
  if (_tempOffset != 0.0)
  {
    _temperature += _tempOffset;
  }

  return AM2315_OK;
}


/////////////////////////////////////////////////////
//
// PRIVATE
//

// return values:

int AM2315::_readSensor()
{
  // EMPTY BUFFER
  for (uint8_t i = 0; i < 5; i++) _bits[i] = 0;

  // HANDLE PENDING IRQ
  yield();
  noInterrupts();

  // WAKE UP the sensor
  _wire->beginTransmission(AM2315_ADDRESS);
  for (int i = 0; i < 10; i++) _wire->write(0);
  int rv = _wire->endTransmission();
  if (rv < 0) return rv;

  // REQUEST DATA
  _wire->beginTransmission(AM2315_ADDRESS);
  _wire->write(0X03);
  _wire->write(0);
  _wire->write(4);
  rv = _wire->endTransmission();
  if (rv < 0) return rv;

  // GET DATA
  const int length = 8;
  int bytes = _wire->requestFrom(AM2315_ADDRESS, length);
  if (bytes == 0)     return AM2315_ERROR_CONNECT;
  if (bytes < length) return AM2315_MISSING_BYTES;

  uint8_t buffer[12];
  for (int i = 0; i < bytes; i++)
  {
    buffer[i] = _wire->read();
  }
  _bits[0] = buffer[2];
  _bits[1] = buffer[3];
  _bits[2] = buffer[4];
  _bits[3] = buffer[5];

  // TEST CHECKSUM HERE
  // return AM2315_ERROR_CHECKSUM;

  return AM2315_OK;
}


// -- END OF FILE --

