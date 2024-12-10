#include "SleepManager.h"

SleepManager::SleepManager(uint32_t sleepDelayMs, gpio_num_t wakeupPin) 
    : sleepDelayMs(sleepDelayMs), wakeupPin(wakeupPin), lastActivityTime(0) {}

void SleepManager::begin() {
    pinMode(wakeupPin, INPUT_PULLUP); // Configure the wake-up pin
    attachInterrupt(digitalPinToInterrupt(wakeupPin), [] {}, FALLING); // Dummy ISR
    esp_sleep_enable_ext0_wakeup(wakeupPin, LOW); // Enable wake-up on LOW signal
    lastActivityTime = millis(); // Initialize the timer
    Serial.println("SleepManager initialized.");
}

void SleepManager::resetSleepTimer() {
    lastActivityTime = millis(); // Update the last activity time
    Serial.println("Sleep timer reset.");
}

void SleepManager::checkSleepCondition() {
    if (millis() - lastActivityTime > sleepDelayMs) {
        Serial.println("Entering deep sleep...");
        delay(100); // Allow Serial messages to send
        esp_deep_sleep_start(); // Enter deep sleep
    }
}
