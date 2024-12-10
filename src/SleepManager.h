#ifndef SLEEPMANAGER_H
#define SLEEPMANAGER_H

#include <Arduino.h>
#include <esp_sleep.h>

class SleepManager {
public:
    SleepManager(uint32_t sleepDelayMs, gpio_num_t wakeupPin);
    void begin();                  // Initialize the sleep manager
    void resetSleepTimer();        // Reset the sleep timer
    void checkSleepCondition();    // Check if the device should enter deep sleep

private:
    uint32_t sleepDelayMs;         // Sleep delay in milliseconds
    uint32_t lastActivityTime;     // Last recorded activity time
    gpio_num_t wakeupPin;          // GPIO pin for the hardware interrupt
};

#endif
