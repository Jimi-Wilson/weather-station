#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

enum HttpMethod {GET, POST};

struct HttpResponse {
    int status = -1;
    String body = "";
    bool success = false;
};

struct HttpRequest {
    HttpMethod method;
    String url;
    const JsonDocument* payload = nullptr;
    String apiKey = "";
};


bool startWifi();
bool isWifiCredsStored();

HttpResponse makeHttpRequest(const HttpRequest& req);

