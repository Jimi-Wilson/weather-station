#include "display/DisplayManager.h"


void DisplayManager::begin() {
    display.begin();
};

void DisplayManager::setScreen(Screen* screen) {
    if (currentScreen != nullptr) {
        currentScreen->onExit();
    }

    currentScreen = screen;

    if (currentScreen != nullptr) {
        currentScreen->onEnter();
    }
};

void DisplayManager::update() {
    if (currentScreen == nullptr) {
        return;
    }
    
    display.clearBuffer();

    currentScreen->draw(display);

    display.sendBuffer();
};

void DisplayManager::powerOff() {
    display.setPowerSave(1);
}
