#ifndef BUTTONATTENDENCE_H
#define BUTTONATTENDENCE_H
#include "StateMachine.h"
#include <Button.h>
#include <Arduino.h>

#define USER_BUTTON GPIO_NUM_4
#define UP_BUTTON GPIO_NUM_15
#define DOWN_BUTTON GPIO_NUM_5

typedef enum
{
    USER_SHORT,
    USER_LONG,
    UP_SHORT,
    DOWN_SHORT
} BUTTON_EVENT;


void button_attendence_init();


static void button_attendence_user_short_press(void *button_handle, void *usr_data);
static void button_attendence_user_long_press(void *button_handle, void *usr_data);
static void button_attendence_up_short_press(void *button_handle, void *usr_data);
static void button_attendence_down_short_press(void *button_handle, void *usr_data);

#endif