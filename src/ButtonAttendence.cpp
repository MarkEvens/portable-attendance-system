#include "ButtonAttendence.h"
#include "StateMachine.h"
void button_attendence_init()
{

    // initializing a button
    Button *btn1 = new Button(USER_BUTTON, false);
    Button *btn2 = new Button(UP_BUTTON, false);
    Button *btn3 = new Button(DOWN_BUTTON, false);

    btn1->attachSingleClickEventCb(&button_attendence_user_short_press, NULL);
    btn1->attachLongPressStartEventCb(&button_attendence_user_long_press, NULL);
    btn2->attachSingleClickEventCb(&button_attendence_up_short_press, NULL);
    btn3->attachSingleClickEventCb(&button_attendence_down_short_press, NULL);
}
void button_attendence_user_short_press(void *button_handle, void *usr_data)
{
     state_machine_button_event_handler(USER_SHORT);
}
void button_attendence_user_long_press(void *button_handle, void *usr_data)
{
    state_machine_button_event_handler(USER_LONG);
}
void button_attendence_up_short_press(void *button_handle, void *usr_data)
{
    state_machine_button_event_handler(UP_SHORT);
}
void button_attendence_down_short_press(void *button_handle, void *usr_data)
{
    state_machine_button_event_handler(DOWN_SHORT);
}