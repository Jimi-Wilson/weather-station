#include <Wire.h>
#include <SPIFFS.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "RTClib.h"

RTC_DATA_ATTR int cycleCount = 0;

RTC_DS3231 rtc;
Adafruit_BME280 bme;

void logSensorReadings();

void uploadData();

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

  esp_sleep_enable_timer_wakeup(150000000ULL);

  if (esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_UNDEFINED)
  {
    logSensorReadings();
  }

  if (cycleCount >= 8)
  {
    uploadData();
  }

  Serial.println("Entering deep sleep...");
  esp_deep_sleep_start();
}

void logSensorReadings()
{
  DateTime now = rtc.now();
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0f;

  char datetime[32];
  sprintf(datetime, "%04d-%02d-%02d %02d:%02d:%02d",
          now.year(), now.month(), now.day(),
          now.hour(), now.minute(), now.second());

  Serial.print("Weather Reading at time: ");
  Serial.println(datetime);

  Serial.print("Temperature: ");
  Serial.println(temperature);

  Serial.print("Humidity: ");
  Serial.println(humidity);

  Serial.print("Pressure: ");
  Serial.println(pressure);

  File dataFile = SPIFFS.open("/datalog.csv", "a");
  if (dataFile)
  {
    char csvLine[128];
    sprintf(csvLine, "%ld,%.2f,%.2f,%.2f",
            now.unixtime(), temperature, humidity, pressure);
    dataFile.println(csvLine);
    dataFile.close();

    cycleCount = (cycleCount + 1) % 8;
  }
  else
  {
    Serial.println("Failed to open datalog.csv for appending");
  }
}

void uploadData() {}

void loop() {}
