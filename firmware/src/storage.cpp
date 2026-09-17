#include <storage.h>
#include <Preferences.h>
#include <ArduinoJson.h>


static Preferences prefs;

RTC_DATA_ATTR Config config;

#define SLEEP_DURATION_BETWEEN_READINGS 150
#define DAY_READINGS_THRESHOLD 8
#define NIGHT_READINGS_THRESHOLD 24
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22
#define MAX_JSON_ROWS 50
#define MAX_LOG_ROWS 500
#define NTP_RESYNC_INTERVAL_SEC 86400UL
#define WIFI_CONNECT_TIMEOUT_ATTEMPTS 10

RTC_DATA_ATTR int cycleCount = 0;
RTC_DATA_ATTR bool uploadPending = false;
RTC_DATA_ATTR uint32_t nextScheduledReading = 0;
RTC_DATA_ATTR uint32_t lastNtpSync = 0;
RTC_DATA_ATTR bool rtcHealthy = true;


bool saveApiKey(const String& apiKey) {
    prefs.begin("auth", false);

    size_t written = prefs.putString("api_key", apiKey);
    
    prefs.end();


    return written > 0;
}


String getApiKey() {
    prefs.begin("auth", true);

    String apiKey = prefs.getString("api_key", "");

    prefs.end();

    return apiKey;
}

bool hasApiKey() {
    return getApiKey().length() > 0;
}

bool saveDeviceClaimed(bool claimed) {
    prefs.begin("auth", false);

    size_t written = prefs.putBool("claimed", claimed);

    prefs.end();
    return written > 0;
}

bool isDeviceClaimedStored() {
    prefs.begin("auth", true);

    bool claimed = prefs.getBool("claimed", false);

    prefs.end();
    return claimed;
}
