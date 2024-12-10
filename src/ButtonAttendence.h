#ifndef BUTTONATTENDENCE_H
#define BUTTONATTENDENCE_H
#include <Button.h>
// for esp32
// #define USER_BUTTON GPIO_NUM_4
// #define UP_BUTTON GPIO_NUM_15
// #define DOWN_BUTTON GPIO_NUM_5

#define USER_BUTTON GPIO_NUM_1
#define UP_BUTTON GPIO_NUM_3
#define DOWN_BUTTON  GPIO_NUM_2
typedef enum
{
    USER_SHORT,
    USER_LONG,
    UP_SHORT,
    DOWN_SHORT,
    UP_LONG_HOLD,
    DOWN_LONG_HOLD
} BUTTON_EVENT;

void button_attendence_init();

void button_attendence_user_short_press(void *button_handle, void *usr_data);
void button_attendence_user_long_press(void *button_handle, void *usr_data);
void button_attendence_up_short_press(void *button_handle, void *usr_data);
void button_attendence_down_short_press(void *button_handle, void *usr_data);
void button_attendence_up_long_press(void *button_handle, void *usr_data);
void button_attendence_down_long_press(void *button_handle, void *usr_data);

#endif