#pragma once

#include <Arduino.h>


bool saveApiKey(const String& apiKey);

String getApiKey();

bool hasApiKey();


struct Config {
    float sleep_duration_between_readings = 150;
    int day_reading_threshold = 8;
    int night_readings_thresholds = 24;
    char apiKey[64] = "";
};
