#include "OledDisplay.h"
#include <Wire.h>
#include <esp_log.h>

// Logging tag
static const char* TAG = "OledDisplay";

// Constructor for OledDisplay
OledDisplay::OledDisplay()
  : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

// Initialize the OLED display
void OledDisplay::init() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    ESP_LOGE(TAG, "OLED initialization failed");
    for (;;)
      ;  // Stay here if initialization fails
  }
  display.clearDisplay();  // Show initial screen
  ESP_LOGI(TAG, "OLED initialized");
  int frame = 0;
  for (int i = 0; i <= 28; i += 1) {
    display.clearDisplay();
    display.drawBitmap(32, 0, frames[frame], FRAME_WIDTH, FRAME_HEIGHT, 1);
    display.display();
    frame = (frame + 1) % FRAME_COUNT;
    delay(FRAME_DELAY);
  }
  display.clearDisplay();
}

// Clear the OLED display
void OledDisplay::clear() {
  display.clearDisplay();
  display.display();  // Update the display after clearing
}
void OledDisplay::fingerLoad() {
  int frame = 0;
  for (int i = 0; i <= 28; i += 1) {
   
    display.drawBitmap(32, 0, finger_load[frame], FRAME_WIDTH, FRAME_HEIGHT, 1);
    display.display();
    frame = (frame + 1) % FRAME_COUNT;
    delay(FRAME_DELAY);
     display.clearDisplay();
  }
}
// Display text on OLED
void OledDisplay::displayText(const char* text) {
  clear();  // Clear the display first
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 20);
  display.print(text);
  display.display();  // Update display to show text
  ESP_LOGI(TAG, "Displayed: %s", text);
}
void OledDisplay::update_battery(const char* text) {

  display.fillRect(100, 0, 27, 15, BLACK);
  display.display();  // Update display to show text
                      // Update every 5 seconds  // Clear the display first
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(100, 4);
  display.print(text);
  display.display();  // Update display to show text
  ESP_LOGI(TAG, "Displayed: %s", text);
}
void OledDisplay::menu_2() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(25, 3);
  display.print("Type");
  display.drawRoundRect(23, 1, 28, 12, 2, WHITE);

  display.drawBitmap(0, 20, epd_bitmap_Student_option, 12, 12, 1);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(17, 22);
  display.print("Student");
  display.drawBitmap(0, 34, epd_bitmap_Faculty_option, 12, 12, 1); 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(17,36);
  display.print("Faculty");// Update every 5 seconds
    display.drawBitmap(0, 48, epd_bitmap_back_option, 12, 12, 1); 
    display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(17, 50);
  display.print("Back");


  display.display();
}
void OledDisplay::menu_1() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(25, 3);
  display.print("Main Menu");
  display.drawRoundRect(23, 1, 60, 12, 2, WHITE);

  display.drawBitmap(0, 20, epd_bitmap_addprints, 12, 12, 1);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(17, 22);
  display.print("Add Prints");
  display.drawBitmap(0, 34, epd_bitmap_Faculty_option, 12, 12, 1); 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(17,36);
  display.print("Faculty");// Update every 5 seconds
    display.drawBitmap(0, 48, epd_bitmap_deleteprints, 12, 12, 1); 
    display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(17, 50);
  display.print("Delete Prints");


  display.display();
}
void OledDisplay::displayicons() {
    display.fillRect(0, 0, 14, 14, BLACK);

   display.drawBitmap(0, 0, battery_icon, 128, 64, 1);
  display.drawBitmap(-70, 0, wifi_icon, 128, 64, 1);
  display.display();
}
void OledDisplay::nowifi_icon() {
    display.fillRect(0, 0, 14, 14, BLACK);

   display.drawBitmap(0, 0, battery_icon, 128, 64, 1);
  display.drawBitmap(-70, 0, no_wifi_icon, 128, 64, 1);
  display.display();
}


void OledDisplay::main_screen() {

  // Update display to show text
  // Update every 5 seconds  // Clear the display first
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20, 30);
  display.print("Place your thumb     for Attendence");
  display.display();  // Update display to show text
}
