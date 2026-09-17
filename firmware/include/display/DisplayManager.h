#pragma once

#include <U8g2lib.h>
#include "display/Screen.h"


class DisplayManager {
    public:
        void begin();
        void setScreen(Screen* screen);
        void update();
        void powerOff();




    private:
        U8G2_SSD1309_128X64_NONAME0_F_HW_I2C display{
        U8G2_R0,
        U8X8_PIN_NONE
    };

    Screen* currentScreen = nullptr;

};
