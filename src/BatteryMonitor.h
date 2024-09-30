#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include "OledDisplay.h"
#include <driver/adc.h>

#define ADC_PIN ADC1_CHANNEL_6  // ADC Channel (GPIO34)
#define MAX_BATTERY_VOLTAGE 4.2 // Maximum battery voltage
#define MIN_BATTERY_VOLTAGE 3.0 // Minimum battery voltage

class BatteryMonitor
{
public:
    BatteryMonitor(OledDisplay &display);
    void init();
    void update();
    void show();

private:
    OledDisplay &oledDisplay;
    void initADC();
    float readBatteryVoltage();
    int calculateBatteryPercentage(float voltage);
};

#endif // BATTERY_MONITOR_H
