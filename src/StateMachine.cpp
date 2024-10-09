#include "StateMachine.h"
#include "ButtonAttendence.h"
#include <Arduino.h>
#include "AttendenceMenu.h"
#include "AttendenceConfig.h"
#include "OledDisplay.h"
extern OledDisplay oledDisplay; // Declare oledDisplay as external, so it's not redefined here

MAIN_STATE menu_state = IDLE;
EVENT_TYPE event = BTN;
BUTTON_EVENT btn_event = UP_SHORT;
MAIN_MENU main_opt = ADD_MENU;
MENU_TYPE menu_type = MENU1;
SUB_MENU sub_opt = STUDENT_SUB_MENU;
const char *buttonEventToString(BUTTON_EVENT event)
{
    switch (event)
    {
    case USER_SHORT:
        return "USER_SHORT";
    case USER_LONG:
        return "USER_LONG";
    case UP_SHORT:
        return "UP_SHORT";
    case DOWN_SHORT:
        return "DOWN_SHORT";
    default:
        return "UNKNOWN_EVENT";
    }
}

void state_machine_button_event_handler(BUTTON_EVENT btn_event)
{
    switch (btn_event)
    {
    case USER_SHORT:

        switch (menu_state)
        {
        case IDLE:
            menu_state = MENU;
            oledDisplay.initMainMenu(main_opt);
            break;
        case MENU:
            state_machine_change_menu_select_event();
            break;
        default: // Added default case
            break;
        }
        break;

    case USER_LONG:
        switch (menu_state)
        {
        case IDLE:
            Serial.println("device going in sleeping mode");
            break;
        case MENU:
            break;
        default: // Added default case
            break;
        }
        break;
        Serial.println("USER_LONG button pressed");

        break;

    case UP_SHORT:
        switch (menu_state)
        {
        case IDLE:
            Serial.println("nothing to do here");
            break;
        case MENU:
            state_machine_change_menu_down_event();
            break;
        default: // Added default case
            break;
        }
        break;
        Serial.println("UP_SHORT");
        break;

    case DOWN_SHORT:
        switch (menu_state)
        {
        case IDLE:
            Serial.println("nothing to do here");
            break;
        case MENU:
            state_machine_change_menu_up_event();
            break;
        default: // Added default case
            break;
        }
        break;
        Serial.println("DOWN_SHORT");
        break;
    default: // Added default case
        break;
    }
}

void state_machine_execute_idle_event(EVENT_TYPE event)
{
    switch (event)
    {
    case BTN:
        Serial.print("event2: ");
        Serial.println(buttonEventToString(btn_event));
        state_machine_idle_button_event_handler(btn_event);

        Serial.println("BTN");

        break;
    case THUMB:

        break;
    case TIMEOUT:
        break;
    default: // Added default case
        break;
    }
}

void state_machine_change_menu_down_event()
{
    switch (menu_type)
    {
    case MENU1:
        switch (main_opt)
        {
        case ADD_MENU:
            main_opt = REMOVE_MENU;
            oledDisplay.initMainMenu(REMOVE_MENU);
            break;
        case REMOVE_MENU:
            main_opt = BACK_MENU;
            oledDisplay.initMainMenu(BACK_MENU);
            break;
        case BACK_MENU:
            main_opt = ADD_MENU;
            oledDisplay.initMainMenu(ADD_MENU);
            break;
        default: // Added default case
            break;
        }
        break;
    case MENU2:
        switch (sub_opt)
        {
        case STUDENT_SUB_MENU:
            sub_opt = FACULTY_SUB_MENU;
            oledDisplay.initSubMenu(main_opt, FACULTY_SUB_MENU);
            break;
        case FACULTY_SUB_MENU:
            sub_opt = BACK_SUB_MENU;
            oledDisplay.initSubMenu(main_opt, BACK_SUB_MENU);
            break;
        case BACK_SUB_MENU:
            sub_opt = STUDENT_SUB_MENU;
            oledDisplay.initSubMenu(main_opt, STUDENT_SUB_MENU);
            break;
        default: // Added default case
            break;
        }
        break;
    default: // Added default case
        break;
    }
}
void state_machine_change_menu_up_event()
{
    switch (menu_type)
    {
    case MENU1:
        switch (main_opt)
        {
        case ADD_MENU:
            main_opt = BACK_MENU;
            oledDisplay.initMainMenu(BACK_MENU);
            break;
        case REMOVE_MENU:
            main_opt = ADD_MENU;
            oledDisplay.initMainMenu(ADD_MENU);
            break;
        case BACK_MENU:
            main_opt = REMOVE_MENU;
            oledDisplay.initMainMenu(REMOVE_MENU);
            break;
        default: // Added default case
            break;
        }
        break;
    case MENU2:
        switch (sub_opt)
        {
        case STUDENT_SUB_MENU:
            sub_opt = BACK_SUB_MENU;
            oledDisplay.initSubMenu(main_opt, BACK_SUB_MENU);
            break;
        case FACULTY_SUB_MENU:
            sub_opt = STUDENT_SUB_MENU;
            oledDisplay.initSubMenu(main_opt, STUDENT_SUB_MENU);
            break;
        case BACK_SUB_MENU:
            sub_opt = FACULTY_SUB_MENU;
            oledDisplay.initSubMenu(main_opt, FACULTY_SUB_MENU);
            break;
        default: // Added default case
            break;
        }
    }
}
void state_machine_change_menu_select_event()
{
    switch (menu_type)
    {
    case MENU1:
        switch (main_opt)
        {
        case ADD_MENU:
            menu_type = MENU2;
            sub_opt = STUDENT_SUB_MENU;
            oledDisplay.initSubMenu(main_opt, STUDENT_SUB_MENU);
            break;
        case REMOVE_MENU:
            menu_type = MENU2;
            sub_opt = STUDENT_SUB_MENU;
            oledDisplay.initSubMenu(main_opt, STUDENT_SUB_MENU);
            break;
        case BACK_MENU:
            menu_state = IDLE;
            oledDisplay.scanThumbInit();
            break;
        default:
            break;
        }
        break; // Add this break to prevent fall-through into MENU2

    case MENU2:
        switch (sub_opt)
        {
        case STUDENT_SUB_MENU:
            Serial.println("STUDENT_SUB_MENU selected");
            break;
        case FACULTY_SUB_MENU:
            Serial.println("FACULTY_SUB_MENU selected");
            break;
        case BACK_SUB_MENU:
            menu_type = MENU1;
            oledDisplay.initMainMenu(ADD_MENU);
            main_opt = ADD_MENU;
            break;
        default:
            break;
        }
        break;

    default: // Ensure there's a default for the outer switch as well
        break;
    }
}
