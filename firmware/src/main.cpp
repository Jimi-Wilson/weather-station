#include <Wire.h>
#include <SPIFFS.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "RTClib.h"

RTC_DS3231 rtc;
Adafruit_BME280 bme;

void setup()
{
  Serial.begin(115200);
  delay(2000);

  // Starting and check I2C devices
  Wire.begin(21, 22);

  if (!bme.begin(0x76))
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1)
      ;
  }
  else
  {
    Serial.println("BME280 Sensor Initialized.");
  }

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC! Check wiring.");
    while (1)
      ;
  }
  else
  {
    Serial.println("DS3231 RTC Initialized.");
  }

  if (rtc.lostPower())
  {
    Serial.println("RTC lost power, resetting time");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Starting storage for logs
  if (!SPIFFS.begin(true))
  {
    Serial.println("An error occurred while mounting SPIFFS");
    return;
  }
  else
  {
    Serial.println("SPIFFS mounted successfully.");
  }

  // Creating datalog.csv file with headers if it doesn't already exist
  if (!SPIFFS.exists("/datalog.csv"))
  {
    File dataFile = SPIFFS.open("/datalog.csv", "a");
    if (dataFile)
    {
      dataFile.println("timestamp,temperature,humidity,pressure");
      dataFile.close();
      Serial.println("Created datalog.csv with headers");
    }
    else
    {
      Serial.println("Failed to create datalog.csv");
    }
  }
}