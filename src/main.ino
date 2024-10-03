#include "BatteryMonitor.h"
#include "OledDisplay.h"
#include "ButtonAttendence.h"

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
    Button *btn = new Button(USER_BUTTON, false);
    Button *btn = new Button(UP_BUTTON, false);
    Button *btn = new Button(DOWN_BUTTON, false);

    btn->attachPressDownEventCb(&onButtonPressDownCb, NULL);
    btn->attachPressUpEventCb(&onButtonPressUpCb, NULL);
    btn->attachPressDownEventCb(&onButtonPressDownRepeatCb, NULL);
    btn->attachSingleClickEventCb(&onButtonSingleClickCb,NULL);
    btn->attachSingleClickEventCb(&onButtonSingleClickRepeatCb,NULL);
    btn->unregisterPressDownEventCb(&onButtonPressDownCb);
    btn->detachSingleClickEvent();
}
