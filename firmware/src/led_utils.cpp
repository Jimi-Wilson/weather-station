#include <Arduino.h> 
#include "led_utils.h"

#define LED_PIN 2

static Ticker ledTicker;
static bool ledState = false;

static void toggleLed() {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
}

void ledSetup() {
    pinMode(LED_PIN, OUTPUT);
}

void startLongBlink() {
    ledTicker.attach(1.0, toggleLed);
}

void startFastBlink() {
    ledTicker.attach(0.15, toggleLed);
}

void stopBlink() {
    ledTicker.detach();
    digitalWrite(LED_PIN, LOW);
}