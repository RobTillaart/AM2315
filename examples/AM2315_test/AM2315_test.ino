//
//    FILE: AM2315_test.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: Demo for AM2315 I2C humidity & temperature sensor
//


#include "AM2315.h"
#include "Wire.h"

AM2315 sensor(&Wire);


void setup()
{
  sensor.begin();

  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("AM2315_LIB_VERSION: ");
  Serial.println(AM2315_LIB_VERSION);
  Serial.println();

  delay(2000);

  Serial.println("Type,\tStatus,\tHumidity (%),\tTemperature (C)");
}


void loop()
{
  if (millis() - sensor.lastRead() >= 2000)
  {
    // READ DATA
    Serial.print("AM2315, \t");
    int status = sensor.read();
    switch (status)
    {
    case AM2315_OK:
      Serial.print("OK,\t");
      break;
    case AM2315_ERROR_CHECKSUM:
      Serial.print("Checksum error,\t");
      break;
    case AM2315_ERROR_CONNECT:
      Serial.print("Connect error,\t");
      break;
    case AM2315_MISSING_BYTES:
      Serial.print("Bytes error,\t");
      break;
    default:
      Serial.print("Unknown error,\t");
      break;
    }
    // DISPLAY DATA, sensor has only one decimal.
    Serial.print(sensor.getHumidity(), 1);
    Serial.print(",\t");
    Serial.println(sensor.getTemperature(), 1);
  }
}


// -- END OF FILE --
