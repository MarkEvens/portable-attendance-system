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
   
    oledDisplay.initSubMenu(REMOVE_MENU,STUDENT_SUB_MENU);
    delay(1000);
    oledDisplay.initSubMenu(REMOVE_MENU,FACULTY_SUB_MENU);
    delay(1000);
    oledDisplay.initSubMenu(REMOVE_MENU,BACK_SUB_MENU);
    delay(1000);
}
