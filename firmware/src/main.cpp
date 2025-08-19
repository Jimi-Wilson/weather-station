#include <Wire.h>
#include <SPIFFS.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "RTClib.h"
#include "driver/rtc_io.h"

#define REED_SWITCH_PIN GPIO_NUM_33

RTC_DATA_ATTR int bucketTipCount = 0;
RTC_DATA_ATTR int cycleCount = 0;

RTC_DS3231 rtc;
Adafruit_BME280 bme;

void handleWakeup();

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

  handleWakeup();

  // When enough data has been recorded, upload data
  if (cycleCount >= 8)
  {
    uploadData();
    cycleCount = 0;
  }

  // Deep sleep configuration
  esp_sleep_enable_timer_wakeup(150000000ULL);

  esp_sleep_enable_ext0_wakeup(REED_SWITCH_PIN, 0);

  gpio_pullup_en(REED_SWITCH_PIN);
  gpio_pulldown_dis(REED_SWITCH_PIN);

  Serial.println("Entering deep sleep...");
  Serial.flush();
  esp_deep_sleep_start();
}

void handleWakeup()
{
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT0:
    Serial.println("Wakeup Reason: Bucket Tip Detected");
    bucketTipCount++;
    break;

  case ESP_SLEEP_WAKEUP_TIMER:
    Serial.println("Wakeup Reason: Timer, Taking Reading");
    logSensorReadings();
    break;

  default:
    Serial.println("Wakeup Reason: Power on / Reset");
    break;
  }
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

    cycleCount++;
  }
  else
  {
    Serial.println("Failed to open datalog.csv for appending");
  }
}

void uploadData() {}

void loop() {}
