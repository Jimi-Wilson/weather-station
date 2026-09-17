#pragma once
#include <Arduino.h>

enum class DeviceClaimStatus {
    UNKNOWN,
    UNCLAIMED,
    CLAIMED,
};

bool registerDevice();

bool isDeviceClaimed();
DeviceClaimStatus getDeviceClaimStatus();
bool getOrCreatePairingCode(String& pairingCode);
