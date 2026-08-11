#include <services.h>
#include <storage.h>
#include <requests.h>
#include <network.h>
#include <config.h>

RegistrationResult registerDevice() {

    RegistrationResult result;
    result.success = false;

    HttpResponse res = makeRegistrationRequest(
        DEVICE_UUID,
        DEVICE_SECRET
    );

    if (!res.success) {
        // Not sure yet maybe launch a webserver and show error in future, but for now just serial out the error
        Serial.println("Failed to register device");
        Serial.println(res.body);

        return result;
    }

    Serial.println("Successfully registered device");

    // Turning response into json obj
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, res.body);

    if (error) {
        Serial.print("Failed to parse registration response: ");
        Serial.println(error.c_str());
        return result;
    }   

    // Getting api key and pairing code, and saving to preferences
    const char* apiKey = doc["api_key"];
    const char* pairingCode = doc["pairing_code"];

    if (!apiKey || !pairingCode) {
        Serial.println("Registration response missing required fields");
        return result;
    }

    if (!saveApiKey(apiKey)) {
        return result;
    };

    Serial.println("API KEY: " + String(apiKey));

    result.pairingCode = pairingCode;
    result.success = true;

    return result;
}