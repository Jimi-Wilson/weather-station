#pragma once

#include "display/Screen.h"

class WifiConnectingScreen : public Screen {
public:
    void draw(U8G2& display) override;
};
