#include <services.h>
#include <storage.h>
#include <requests.h>
#include <network.h>
#include <config.h>

bool registerDevice() {
    HttpResponse res = makeRegistrationRequest(
        DEVICE_UUID,
        DEVICE_SECRET
    );

    if (!res.success) {
        // Not sure yet maybe launch a webserver and show error in future, but for now just serial out the error
        Serial.println("Failed to register device");
        Serial.println(res.body);

        return false;
    }

    Serial.println("Successfully registered device");

    // Turning response into json obj
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, res.body);

    if (error) {
        Serial.print("Failed to parse registration response: ");
        Serial.println(error.c_str());
        return false;
    }   

    if (res.success == false) {
        return false;
    }

    // Getting api key and pairing code, and saving to preferences
    const char* apiKey = doc["api_key"];

    if (!apiKey) {
        Serial.println("Registration response missing required fields");
        return false;
    }

    if (!saveApiKey(apiKey)) {
        return false;
    };

    Serial.println("API KEY: " + String(apiKey));

    return true;
}


bool isDeviceClaimed() {
    // Make a request of the server to get the pairing status of the device
    HttpResponse res = isDevicedClaimedRequest();

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, res.body);

    if (error) {
        Serial.println("Failed to parse isDeviceClaimed response: ");
        Serial.println(error.c_str());
        Serial.println(res.body);
        return false;
    }

    if (res.success == false) {
        return false;
    }


    const char* pairingStatus = doc["status"] | "";

    if (strcmp(pairingStatus, "unclaimed") == 0) {
        return false;
    }

    return true;

}