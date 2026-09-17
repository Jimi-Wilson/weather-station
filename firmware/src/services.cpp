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

    const char* apiKey = doc["api_key"];

    if (!apiKey) {
        Serial.println("Registration response missing required fields");
        return false;
    }

    if (!saveApiKey(apiKey)) {
        return false;
    };

    Serial.println("API key saved");

    return true;
}


namespace {
bool parsePairingCode(const HttpResponse& res, String& pairingCode) {
    if (!res.success) {
        return false;
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, res.body);
    if (error) {
        Serial.print("Failed to parse pairing-code response: ");
        Serial.println(error.c_str());
        return false;
    }

    const char* code = doc["pairing_code"] | "";
    if (code[0] == '\0') {
        Serial.println("Pairing-code response did not contain a code");
        return false;
    }

    pairingCode = code;
    return true;
}
}  // namespace


bool getOrCreatePairingCode(String& pairingCode) {
    HttpResponse res = getPairingCodeRequest();

    if (res.status == 404) {
        res = createPairingCodeRequest();
    }

    if (!parsePairingCode(res, pairingCode)) {
        Serial.println("Failed to obtain a pairing code");
        if (res.body.length() > 0) {
            Serial.println(res.body);
        }
        return false;
    }

    return true;
}


DeviceClaimStatus getDeviceClaimStatus() {
    HttpResponse res = isDeviceClaimedRequest();
    if (!res.success) {
        return DeviceClaimStatus::UNKNOWN;
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, res.body);
    if (error) {
        Serial.print("Failed to parse pairing-status response: ");
        Serial.println(error.c_str());
        return DeviceClaimStatus::UNKNOWN;
    }

    const char* status = doc["status"] | "";
    if (strcmp(status, "claimed") == 0) {
        return DeviceClaimStatus::CLAIMED;
    }
    if (strcmp(status, "unclaimed") == 0) {
        return DeviceClaimStatus::UNCLAIMED;
    }

    Serial.println("Pairing-status response contained an unknown status");
    return DeviceClaimStatus::UNKNOWN;
}


bool isDeviceClaimed() {
    return getDeviceClaimStatus() == DeviceClaimStatus::CLAIMED;
}
