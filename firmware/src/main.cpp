#include <Wire.h>
#include <LittleFS.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "RTClib.h"
#include "driver/rtc_io.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "config.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

#define REED_SWITCH_PIN GPIO_NUM_33
#define SLEEP_DURATION_BETWEEN_READINGS 150
#define DAY_READINGS_THRESHOLD 8
#define NIGHT_READINGS_THRESHOLD 24
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

RTC_DATA_ATTR int bucketTipCount = 0;
RTC_DATA_ATTR int cycleCount = 0;
RTC_DATA_ATTR bool uploadPending = false;
RTC_DATA_ATTR uint32_t nextScheduledReading = 0;

RTC_DS3231 rtc;
Adafruit_BME280 bme;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0);

void handleWakeup(esp_sleep_wakeup_cause_t reason);
void logSensorReadings();
void setupDatalogFile();
void parseDatalogFile(JsonDocument &doc);
bool uploadData();
bool connectToWiFi();
void syncTimeFromNTP();

void setup()
{
  Serial.begin(115200);
  delay(2000);

  // Starting and checking I2C devices
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

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
    syncTimeFromNTP();
  }

  setupDatalogFile();

  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  handleWakeup(wakeup_reason);

  uint64_t sleepDuration = SLEEP_DURATION_BETWEEN_READINGS;
  DateTime now = rtc.now();

  if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT0)
  {
    if (nextScheduledReading > now.unixtime())
    {
      sleepDuration = nextScheduledReading - now.unixtime();
    }
    else
    {
      wakeup_reason = ESP_SLEEP_WAKEUP_TIMER;
    }
  }

  if (wakeup_reason != ESP_SLEEP_WAKEUP_EXT0)
  {
    logSensorReadings();

    nextScheduledReading = rtc.now().unixtime() + SLEEP_DURATION_BETWEEN_READINGS;

    int currentHour = now.hour();
    bool isNightTime = (currentHour >= 22 || currentHour < 6);
    int uploadThreshold = isNightTime ? NIGHT_READINGS_THRESHOLD : DAY_READINGS_THRESHOLD;

    if (cycleCount >= uploadThreshold || uploadPending)
    {
      if (uploadData())
      {
        cycleCount = 0;
        bucketTipCount = 0;
        uploadPending = false;
        LittleFS.remove("/datalog.csv");
        setupDatalogFile();
      }
      else
      {
        uploadPending = true;
      }
    }
  }

  // Deep sleep configuration
  if (sleepDuration < 2)
  {
    sleepDuration = 2;
  }

  esp_sleep_enable_timer_wakeup(sleepDuration * 1000000ULL);
  esp_sleep_enable_ext0_wakeup(REED_SWITCH_PIN, 0);
  gpio_pullup_en(REED_SWITCH_PIN);
  gpio_pulldown_dis(REED_SWITCH_PIN);

  Serial.println("Entering deep sleep...");
  Serial.flush();
  esp_deep_sleep_start();
}

void handleWakeup(esp_sleep_wakeup_cause_t reason)
{

  switch (reason)
  {
  case ESP_SLEEP_WAKEUP_EXT0:
    Serial.println("Wakeup Reason: Bucket Tip Detected");
    bucketTipCount++;
    break;

  case ESP_SLEEP_WAKEUP_TIMER:
    Serial.println("Wakeup Reason: Timer, Taking Reading");
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
  isnan(temperature) ? Serial.println("FAILED") : Serial.println(temperature);
  Serial.print("Humidity: ");
  (isnan(humidity) || humidity == 0.0f) ? Serial.println("FAILED (Invalid Value)") : Serial.println(humidity);
  Serial.print("Pressure: ");
  isnan(pressure) ? Serial.println("FAILED") : Serial.println(pressure);

  File dataFile = LittleFS.open("/datalog.csv", "a");
  if (dataFile)
  {
    char tempStr[10] = "";
    char humStr[10] = "";
    char presStr[12] = "";

    if (!isnan(temperature))
    {
      dtostrf(temperature, 1, 2, tempStr);
    }

    if (!isnan(humidity) && humidity > 0.0f)
    {
      dtostrf(humidity, 1, 2, humStr);
    }

    if (!isnan(pressure))
    {
      dtostrf(pressure, 1, 2, presStr);
    }

    dataFile.printf("%ld,%s,%s,%s\n", now.unixtime(), tempStr, humStr, presStr);
    dataFile.close();

    cycleCount++;
  }
  else
  {
    Serial.println("Failed to open datalog.csv for appending");
  }
}

void setupDatalogFile()
{
  // Starting storage for logs
  if (!LittleFS.begin(true))
  {
    Serial.println("An error occurred while mounting LittleFS");
    return;
  }
  else
  {
    Serial.println("LittleFS mounted successfully.");
  }

  // Creating datalog.csv file with headers if it doesn't already exist
  if (!LittleFS.exists("/datalog.csv"))
  {
    File dataFile = LittleFS.open("/datalog.csv", "a");
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

void parseDatalogFile(JsonDocument &doc)
{
  File dataFile = LittleFS.open("/datalog.csv", "r");
  if (!dataFile)
  {
    Serial.println("Failed to open datalog for reading");
    return;
  }

  JsonArray readings = doc["readings"].to<JsonArray>();

  // Skipping headers
  if (dataFile.available())
  {
    dataFile.readStringUntil('\n');
  }

  // Parsing csv for lines
  while (dataFile.available())
  {
    String line = dataFile.readStringUntil('\n');
    line.trim();

    if (line.length() == 0)
      continue;

    JsonObject reading = readings.add<JsonObject>();

    int indexOfFirstComma = line.indexOf(',');
    int indexOfSeccondComma = line.indexOf(',', indexOfFirstComma + 1);
    int indexOfThirdComma = line.indexOf(',', indexOfSeccondComma + 1);

    String timeString = line.substring(0, indexOfFirstComma);
    String temperatureString = line.substring(indexOfFirstComma + 1, indexOfSeccondComma);
    String humidityString = line.substring(indexOfSeccondComma + 1, indexOfThirdComma);
    String pressureString = line.substring(indexOfThirdComma + 1);

    // Adding reading to JSON
    reading["timestamp"] = atol(timeString.c_str());

    if (temperatureString.length() == 0)
    {
      reading["temperature"] = serialized(nullptr);
    }
    else
    {
      reading["temperature"] = temperatureString.toFloat();
    }

    if (humidityString.length() == 0)
    {
      reading["humidity"] = serialized(nullptr);
    }
    else
    {
      reading["humidity"] = humidityString.toFloat();
    }

    if (pressureString.length() == 0)
    {
      reading["pressure"] = serialized(nullptr);
    }
    else
    {
      reading["pressure"] = pressureString.toFloat();
    }
  }
  dataFile.close();
}

bool uploadData()
{
  JsonDocument doc;

  if (!connectToWiFi())
  {
    Serial.println("Unable to connect to WiFi");
    return false;
  }

  parseDatalogFile(doc);

  doc["device_id"] = DEVICE_ID;

  doc["bucket_tips"] = bucketTipCount;

  String jsonPayload;
  serializeJson(doc, jsonPayload);

  Serial.println("Uploading data...");
  Serial.println(jsonPayload);

  // Setting up http request
  HTTPClient http;
  http.begin(API_ENDPOINT);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", String("Api-Key ") + API_KEY);
  int httpResponseCode = http.POST(jsonPayload);

  // Handle http response
  if (httpResponseCode > 0)
  {
    Serial.printf("HTTP Response code: %d\n", httpResponseCode);
    String responsePayload = http.getString();
    Serial.println(responsePayload);

    Serial.println("Data upload successful. Re-syncing RTC time...");
    syncTimeFromNTP();
  }
  else
  {
    Serial.printf("Error code: %d\n", httpResponseCode);
  }

  http.end();
  WiFi.disconnect(true);
  Serial.println("WiFi disconnected.");

  if (httpResponseCode >= 200 && httpResponseCode < 300)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool connectToWiFi()
{
  Serial.print("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  int timeoutCounter = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println(".");
    timeoutCounter++;
    if (timeoutCounter > 30)
    {
      Serial.println("\nConnection timed out");
      return false;
    }
  }
  Serial.println("\nConnected to WiFi");
  return true;
}

void syncTimeFromNTP()
{
  Serial.println("Attempting to sync RTC with NTP server...");
  bool wifiWasOff = (WiFi.status() != WL_CONNECTED);
  if (wifiWasOff)
  {
    if (!connectToWiFi())
    {
      Serial.println("Cannot sync time, WiFi connection failed.");
      return;
    }
  }
  else
  {
    Serial.println("WiFi already connected, proceeding with time sync.");
  }

  timeClient.begin();
  Serial.println("Updating time from NTP server...");
  if (timeClient.forceUpdate())
  {
    unsigned long epochTime = timeClient.getEpochTime();
    rtc.adjust(DateTime(epochTime));

    DateTime now = rtc.now();
    Serial.print("RTC time successfully synced to (UTC): ");
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.println(now.second(), DEC);
  }
  else
  {
    Serial.println("Failed to get time from NTP server.");
  }

  if (wifiWasOff)
  {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    Serial.println("WiFi disconnected after time sync.");
  }
}

void loop() {}
