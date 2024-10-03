#include "BatteryMonitor.h"
#include "OledDisplay.h"
#include "ButtonAttendence.h"
#include "StateMachine.h"

OledDisplay oledDisplay;
BatteryMonitor batteryMonitor(oledDisplay);

void setup()
{
    Serial.begin(115200);

    // Initialize the display and battery monitor
    oledDisplay.init();
    batteryMonitor.init();
    button_attendence_init();
}

void loop()
{
    delay(10);
}
