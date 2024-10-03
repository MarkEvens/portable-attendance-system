#include "StateMachine.h"
#include "ButtonAttendence.h"

void state_machine_execute_main_event(BUTTON_EVENT btn_event)
{
    switch (btn_event)
    {
    case USER_SHORT:
        Serial.println("User Short Press");
        break;
    case USER_LONG:
        Serial.println("User Long Press");
        break;
    case UP_SHORT:
        Serial.println("Up Short Press");
        break;
    case DOWN_SHORT:
        Serial.println("Down Short Press");
        break;
    default:
        break;
    }
}