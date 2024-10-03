#include "StateMachine.h"
#include "ButtonAttendence.h"
#include <Arduino.h>
void state_machine_execute_main_event(BUTTON_EVENT btn_event)
{
    switch (btn_event)
    {
    case USER_SHORT:
        Serial.println("USER_SHORT");
        break;
    case USER_LONG:
        Serial.println("USER_LONG");
        break;
    case UP_SHORT:
        Serial.println("UP_SHORT");
        break;
    case DOWN_SHORT:
        Serial.println("DOWN_SHORT");
        break;
    }
}