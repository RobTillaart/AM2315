//
//    FILE: unit_test_001.cpp
//  AUTHOR: Rob Tillaart
//    DATE: 2022-01-05
// PURPOSE: unit tests for the DHT12 library
//          https://github.com/RobTillaart/DHT12
//          https://github.com/Arduino-CI/arduino_ci/blob/master/REFERENCE.md
//

// supported assertions
// https://github.com/Arduino-CI/arduino_ci/blob/master/cpp/unittest/Assertion.h#L33-L42
// ----------------------------
// assertEqual(expected, actual)
// assertNotEqual(expected, actual)
// assertLess(expected, actual)
// assertMore(expected, actual)
// assertLessOrEqual(expected, actual)
// assertMoreOrEqual(expected, actual)
// assertTrue(actual)
// assertFalse(actual)
// assertNull(actual)
// assertNotNull(actual)


#include <ArduinoUnitTests.h>


#include "Wire.h"
#include "AM2315.h"


unittest_setup()
{
  fprintf(stderr, "AM2315_LIB_VERSION: %s \n", (char *) AM2315_LIB_VERSION);
}

unittest_teardown()
{
}


unittest(test_constants)
{
  assertEqual(  0, AM2315_OK);
  assertEqual(-10, AM2315_ERROR_CHECKSUM);
  assertEqual(-11, AM2315_ERROR_CONNECT);
  assertEqual(-12, AM2315_MISSING_BYTES);
}


unittest(test_constructor)
{
  AM2315 sensor(&Wire);

  assertEqualFloat(0, sensor.getTemperature(), 0.001);
  assertEqualFloat(0, sensor.getHumidity(), 0.001);
  assertEqualFloat(0, sensor.getTempOffset(), 0.001);
  assertEqualFloat(0, sensor.getHumOffset(), 0.001);

  sensor.begin();
  assertEqual(AM2315_ERROR_CONNECT, DHT.read());

  assertEqualFloat(0, sensor.getTemperature(), 0.001);
  assertEqualFloat(0, sensor.getHumidity(), 0.001);
  assertEqualFloat(0, sensor.getTempOffset(), 0.001);
  assertEqualFloat(0, sensor.getHumOffset(), 0.001);
}


unittest(test_offset)
{
  AM2315 sensor(&Wire);
  sensor.begin();

  assertEqualFloat(0, sensor.getTempOffset(), 0.001);
  assertEqualFloat(0, sensor.getHumOffset(), 0.001);

  for (float offset = -1.5; offset < 1.5; offset += 0.5)
  {
    sensor.setHumOffset(offset);
    sensor.setTempOffset(offset);

    assertEqualFloat(offset, sensor.getTempOffset(), 0.001);
    assertEqualFloat(offset, sensor.getHumOffset(), 0.001);

    assertEqualFloat(offset, sensor.getTemperature(), 0.001);
    assertEqualFloat(offset, sensor.getHumidity(), 0.001);
  }
}


unittest_main()

// --------
