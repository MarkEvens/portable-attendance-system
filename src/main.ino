#include "BatteryMonitor.h"
#include "OledDisplay.h"
#include "ButtonAttendence.h"
#include "StateMachine.h"
#include "FingerPrint.h"

OledDisplay oledDisplay;
BatteryMonitor batteryMonitor(oledDisplay);

void setup()
{
    Serial.begin(115200);
    pinMode(2, OUTPUT);
    // Initialize the display and battery monitor
    oledDisplay.init();
    batteryMonitor.init();
    button_attendence_init();
    oledDisplay.scanThumbInit();
    Fingerprint_init();
}

void loop()
{
    digitalWrite(2, HIGH);
    delay(1000);
    digitalWrite(2, LOW);
    delay(1000);
}
