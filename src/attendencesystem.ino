#include "BatteryMonitor.h"
#include "OledDisplay.h"

OledDisplay oledDisplay;
BatteryMonitor batteryMonitor(oledDisplay);

void setup()
{
    Serial.begin(115200);

    // Initialize the display and battery monitor
    oledDisplay.init();
    batteryMonitor.init();
}

void loop()
{
    // oledDisplay.menu_2();
    // oledDisplay.displayicons();
    // delay(2000);
    // oledDisplay.menu_1();
    // oledDisplay.displayicons();

    oledDisplay.SuccessfulInit(ADD,STUDENT, 23);
    delay(2000);
    oledDisplay.SuccessfulInit(REMOVE ,FACULTY,123);
    delay(2000);
    oledDisplay.SuccessfulInit(ADD,FACULTY, 23);
    delay(2000);
    oledDisplay.SuccessfulInit(REMOVE ,STUDENT,123);
    delay(2000);
}
