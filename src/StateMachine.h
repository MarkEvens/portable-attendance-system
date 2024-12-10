#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
#include "ButtonAttendence.h"
typedef enum
{
    IDLE,
    MENU,
    READ
} MAIN_STATE;

typedef enum
{
    BTN,
    THUMB,
    TIMEOUT
} EVENT_TYPE;
typedef enum
{
  ON,
  OFF
} ATTEN_MODE;
void state_machine_execute_main_event();
void state_machine_button_event_handler(BUTTON_EVENT btn_event);
void state_machine_idle_button_event_handler(BUTTON_EVENT btn_event);
void state_machine_change_menu_up_event();
void state_machine_change_menu_down_event();
void state_machine_change_menu_select_event();
MAIN_STATE getMenuState();
ATTEN_MODE getattendencemode();


void state_machine_execute_idle_event(EVENT_TYPE event);

#endif