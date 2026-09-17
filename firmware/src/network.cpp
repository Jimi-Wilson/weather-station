#include <network.h>
#include <WiFiManager.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <config.h>

static WiFiManager wifiManager;

HttpResponse makeHttpRequest(const HttpRequest& req) {
    HttpResponse res;

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[HTTP] Request aborted: Wi-Fi not connected.");
        return res;
    }

    HTTPClient http;
    http.begin(req.url);
    http.setConnectTimeout(2500);
    http.setTimeout(2500);

    // Attaching apikey, if provided
    if (req.apiKey.length() > 0) {
        http.addHeader("X-Device-Key", req.apiKey);
    }

    // Processing request based on method
    if (req.method == HttpMethod::POST) {
        http.addHeader("Content-Type", "application/json");

        String jsonPayload = "";
        if (req.payload != nullptr) {
            serializeJson(*req.payload, jsonPayload);
        }

        res.status = http.POST(jsonPayload);
    }
    else if (req.method == HttpMethod::GET) {
        res.status = http.GET();
    }

    if (res.status > 0) {
        res.body = http.getString();
        res.success = (res.status >= 200 && res.status < 300);
    } else {
        Serial.printf("[HTTP] Request failed. Error: %s\n", http.errorToString(res.status).c_str());
    }

    http.end();
    return res;
}



bool startWifi(){

    wifiManager.setTitle("Weather Station WIFI Setup");


    bool connected = wifiManager.autoConnect("weather-station", "12345678");
    if (connected) {
        WiFi.setAutoReconnect(true);
    }

    return connected;
}


bool isWifiCredsStored() {
    return wifiManager.getWiFiIsSaved();
}
