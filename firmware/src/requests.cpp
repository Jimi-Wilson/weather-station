#include <network.h>
#include <config.h>
#include <storage.h>


HttpResponse makeRegistrationRequest(const String& deviceId, const String& registrationSecret) {
    // Preparing payload
    JsonDocument payload;

    payload["device_id"] = deviceId;
    payload["registration_secret"] = registrationSecret;

    // Building request
    HttpRequest req;

    req.method = HttpMethod::POST;
    req.url = String(API_BASE_URL) + "/device/register";
    req.payload = &payload;

    // Make request
    HttpResponse res = makeHttpRequest(req);

    return res;
}



HttpResponse getPairingCodeRequest() {
    HttpRequest req;

    req.method = HttpMethod::GET;
    req.url = String(API_BASE_URL) + "/device/pairing-code";
    req.apiKey = getApiKey();

    return makeHttpRequest(req);
}


HttpResponse createPairingCodeRequest() {
    HttpRequest req;

    req.method = HttpMethod::POST;
    req.url = String(API_BASE_URL) + "/device/pairing-code";
    req.apiKey = getApiKey();

    return makeHttpRequest(req);
}


HttpResponse isDeviceClaimedRequest() {
    HttpRequest req;

    req.method = HttpMethod::GET;
    req.url = String(API_BASE_URL) + "/device/pairing-code/status";
    req.apiKey = getApiKey();

    return makeHttpRequest(req);
}
