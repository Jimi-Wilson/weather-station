#pragma once

#include <network.h>

HttpResponse makeRegistrationRequest(
    const String& deviceId,
    const String& registrationSecret
);