#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Adafruit_SSD1306.h>
#include "AttendenceConfig.h"
#include "AttendenceMenu.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin (-1 if sharing Arduino reset pin)
#define OLED_ADDR 0x3C   // OLED I2C address

class OledDisplay
{
public:
    OledDisplay(void); 

    void init(void);
    
    void splashScreenInit(void);
    
    void clear(void);
    
    void displayText(const char *text);
    
    void initSubMenu(MAIN_MENU main_opt, SUB_MENU opt);
    
    void initMainMenu(MAIN_MENU opt);
    
    void presentScreenInit(USER_TYPE user_type, int enroll_no);
    
    void removeUserEnter(USER_TYPE user_type, int enroll_no);
    
    void SuccessfulInit(OP_TYPE op_type,USER_TYPE user_type, int enroll_no);
    
    void update_battery(const char *text);
    
    void removeNotAvailableInit(USER_TYPE user_type);
    
    void addUserEnterNew(USER_TYPE user_type, int enroll_no);
    
    void main_screen(void);
    
    void SCAN_LOAD_ANIMATION(void);
    
    void wifi_icons(void);
    
    void nowifi_icon(void);
    
    void scanThumbInit(void);
    
    void scanThumbForAuth(void);
    
    void invalidUserInit(void);
    
    void authFailedInit(void);
    
    void attendence_sucess(const int enno);

    void number_select_faculty(const int enno);

private:
    Adafruit_SSD1306 display;
};



#endif // OLED_DISPLAY_H
