#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
#include "ButtonAttendence.h"

typedef enum
{
    IDEAL,
    MENU,
    OFF
} STATE_MACHINE_STATE;

void state_machine_execute_main_event(BUTTON_EVENT btn_event);

#endif