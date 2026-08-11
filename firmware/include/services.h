#pragma once
#include <Arduino.h>

struct RegistrationResult {
    bool success;
    String pairingCode;
};



RegistrationResult registerDevice();