#include <network.h>
#include <requests.h>
#include <storage.h>
#include <services.h>
#include <led_utils.h>

void setup() {
  Serial.begin(115200);

  ledSetup();

  bool needsSetup = !isWifiCredsStored() || !hasApiKey();

  if (needsSetup) {
    Serial.println("Needs setup");
    startFastBlink();
    startWifi();
    stopBlink();


    if (!hasApiKey()) {
      bool result = registerDevice();
      if (result) {
        Serial.println("Device Registered");
      }
      // TODO: handle if register device fails from a mismatch in id and secrete 
    }

    if (!isDeviceClaimed()) {
      // Start claiming process
      Serial.println("Device Not Paired, starting pairing procedure...");

    }
  }



  // Check if wifi creds are stored.

  // If not open connecting page

  // Request for apikey, if not stored

  // Display claim code on web page, running in STA and AP mode.
        // Have a refresh code button
        // Have a ive linked my account
        // Polling a `status` endpoint every 1 min / 30s


  // Status endpoint (HTTP) for telling if claimed, for configuration updates, OTA updates and remote comands (probably not)
  


}


void loop() {}