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

#define SLEEP_DURATION_BETWEEN_READINGS 150
#define DAY_READINGS_THRESHOLD 8
#define NIGHT_READINGS_THRESHOLD 24
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22
#define MAX_JSON_ROWS 50

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
  #ifdef DEBUG
    Serial.begin(115200);
  #endif


  // Starting and checking I2C devices
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  if (!bme.begin(0x76))
  {
    #ifdef DEBUG
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
    #endif
  }
  else
  {
    bme.setSampling(Adafruit_BME280::MODE_FORCED,
                    Adafruit_BME280::SAMPLING_X1, // Temperature
                    Adafruit_BME280::SAMPLING_X1, // Pressure
                    Adafruit_BME280::SAMPLING_X1, // Humidity
                    Adafruit_BME280::FILTER_OFF);

                    
  }

  if (!rtc.begin())
  {
    #ifdef DEBUG
      Serial.println("Couldn't find RTC! Check wiring.");
    #endif
  }

  if (rtc.lostPower())
  {
    syncTimeFromNTP();
  }

  setupDatalogFile();

  uint64_t sleepDuration = SLEEP_DURATION_BETWEEN_READINGS;
  DateTime now = rtc.now();

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
      uploadPending = false;
      LittleFS.remove("/datalog.csv");
      setupDatalogFile();
    }
    else
    {
      uploadPending = true;
    }
  }


  // Deep sleep configuration
  if (sleepDuration < 2)
  {
    sleepDuration = 2;
  }

  esp_sleep_enable_timer_wakeup(sleepDuration * 1000000ULL);
  #ifdef DEBUG
    Serial.flush();
  #endif
  esp_deep_sleep_start();
}

void logSensorReadings()
{
  bme.takeForcedMeasurement();

  DateTime now = rtc.now();
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0f;

  char datetime[32];
  sprintf(datetime, "%04d-%02d-%02d %02d:%02d:%02d",
          now.year(), now.month(), now.day(),
          now.hour(), now.minute(), now.second());

  #ifdef DEBUG
    Serial.print("Weather Reading at time: ");
    Serial.println(datetime);

    Serial.print("Temperature: ");
    isnan(temperature) ? Serial.println("FAILED") : Serial.println(temperature);
    Serial.print("Humidity: ");
    (isnan(humidity) || humidity == 0.0f) ? Serial.println("FAILED (Invalid Value)") : Serial.println(humidity);
    Serial.print("Pressure: ");
    isnan(pressure) ? Serial.println("FAILED") : Serial.println(pressure);
  #endif

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
    #ifdef DEBUG
      Serial.println("Failed to open datalog.csv for appending");
    #endif
  }
}

void setupDatalogFile()
{
  // Starting storage for logs
  if (!LittleFS.begin(true))
  {
    #ifdef DEBUG
      Serial.println("An error occurred while mounting LittleFS");
    #endif
    return;
  }

  // Creating datalog.csv file with headers if it doesn't already exist
  if (!LittleFS.exists("/datalog.csv"))
  {
    File dataFile = LittleFS.open("/datalog.csv", "a");
    if (dataFile)
    {
      dataFile.println("timestamp,temperature,humidity,pressure");
      dataFile.close();
    }
  }
}

void parseDatalogFile(JsonDocument &doc)
{
  File dataFile = LittleFS.open("/datalog.csv", "r");
  if (!dataFile)
  {
    return;
  }

  JsonArray readings = doc["readings"].to<JsonArray>();

  // Skipping headers
  if (dataFile.available())
  {
    dataFile.readStringUntil('\n');
  }

  int rowsProcessed = 0;

  // Parsing csv for lines
  while (dataFile.available() && rowsProcessed < MAX_JSON_ROWS) {
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
      reading["temperature"] = nullptr;
    }
    else
    {
      reading["temperature"] = temperatureString.toFloat();
    }

    if (humidityString.length() == 0)
    {
      reading["humidity"] = nullptr;
    }
    else
    {
      reading["humidity"] = humidityString.toFloat();
    }

    if (pressureString.length() == 0)
    {
      reading["pressure"] = nullptr;
    }
    else
    {
      reading["pressure"] = pressureString.toFloat();
    }
    rowsProcessed++;
  }
  dataFile.close();
}

bool uploadData()
{
  JsonDocument doc;

  if (!connectToWiFi())
  {
    #ifdef DEBUG
      Serial.println("Unable to connect to WiFi");
    #endif
    return false;
  }

  parseDatalogFile(doc);

  doc["device_id"] = DEVICE_ID;
  doc["bucket_tips"] = 0;

  String jsonPayload;
  serializeJson(doc, jsonPayload);

  #ifdef DEBUG
    Serial.println(jsonPayload);
    Serial.println("Uploading data...");
  #endif

  // Setting up http request
  HTTPClient http;
  http.begin(API_ENDPOINT);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", String("Api-Key ") + API_KEY);
  int httpResponseCode = http.POST(jsonPayload);

  #ifdef DEBUG
    Serial.printf("HTTP Response code: %d\n", httpResponseCode);
  #endif

  // Handle http response
  if (httpResponseCode > 0)
  {
    #ifdef DEBUG
      Serial.printf("HTTP Response code: %d\n", httpResponseCode);
    #endif
    String responsePayload = http.getString();

    #ifdef DEBUG
      Serial.println("Data upload successful. Re-syncing RTC time...");
    #endif
    syncTimeFromNTP();
  }
  else
  {
    #ifdef DEBUG
      Serial.printf("Error code: %d\n", httpResponseCode);
    #endif
  }

  http.end();
  WiFi.disconnect(true);
  #ifdef DEBUG
    Serial.println("WiFi disconnected.");
  #endif

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
  #ifdef DEBUG
    Serial.print("Connecting to WiFi...");
  #endif
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  int timeoutCounter = 0;
  while (WiFi.status() != WL_CONNECTED && timeoutCounter < 20) { 
    delay(500);
    timeoutCounter++;
  }
  return WiFi.status() == WL_CONNECTED;
}

void syncTimeFromNTP()
{
  #ifdef DEBUG
    Serial.println("Attempting to sync RTC with NTP server...");
  #endif
  bool wifiWasOff = (WiFi.status() != WL_CONNECTED);
  if (wifiWasOff && !connectToWiFi())
  {
    return;

  }
  timeClient.begin();
  #ifdef DEBUG
    Serial.println("Updating time from NTP server...");
  #endif
  if (timeClient.forceUpdate())
  {
    unsigned long epochTime = timeClient.getEpochTime();
    rtc.adjust(DateTime(epochTime));

    DateTime now = rtc.now();
    #ifdef DEBUG
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
    #endif
  }
  else
  {
    #ifdef DEBUG
      Serial.println("Failed to get time from NTP server.");
    #endif

  }

  if (wifiWasOff)
  {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    #ifdef DEBUG
      Serial.println("WiFi disconnected after time sync.");
    #endif

  }
}

void loop() {}
