/***************************************************
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/


#include <Adafruit_Fingerprint.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char* ssid = "J";
const char* password = "9023669791";
int screen = 0;
int up_clicked = 0;
int down_clicked = 0;
int ok_clicked = 0;
int fcount;
uint8_t id;



#define mySerial Serial2


int end = 1;
String datalink = "https://script.google.com/macros/s/AKfycbykOAbbBFIMg6mQt_qT6oHlw5kaVNZjIfPSO66XiVV_ESBbP4uc7QUpx3AGYmCAJeyy/exec?1=1";
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int y;
String load;
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

static const uint8_t delete_prints_img[1024] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x81, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x04, 0x18, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x04, 0x18, 0x20, 0x70, 0x10, 0x20, 0x78, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x18, 0x10, 0x88, 0x10, 0x20, 0x44, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x18, 0x10, 0x88, 0xf1, 0xe0, 0x44, 0x98, 0x8f, 0x1e, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x09, 0xff, 0x90, 0x89, 0x12, 0x20, 0x44, 0xa0, 0x88, 0x88, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x09, 0xff, 0x90, 0x89, 0x12, 0x20, 0x78, 0xc0, 0x88, 0x88, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x18, 0x10, 0xf9, 0x12, 0x20, 0x40, 0x80, 0x88, 0x88, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x18, 0x10, 0x89, 0x12, 0x20, 0x40, 0x80, 0x88, 0x88, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x04, 0x18, 0x20, 0x89, 0x12, 0x20, 0x40, 0x80, 0x88, 0x88, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x04, 0x18, 0x20, 0x88, 0xf1, 0xe0, 0x40, 0x80, 0x88, 0x86, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x81, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00,
  0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00,
  0x40, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x40, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x43, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x44, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x47, 0xff, 0xe0, 0xf8, 0x00, 0x60, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x30, 0x00, 0x00, 0x06, 0x00,
  0x40, 0x00, 0x00, 0xcc, 0x00, 0x60, 0x01, 0x80, 0x00, 0x66, 0x00, 0x00, 0x00, 0xc0, 0x06, 0x00,
  0x42, 0x42, 0x40, 0xcc, 0x78, 0x61, 0xe3, 0xe3, 0xc0, 0x66, 0x66, 0x31, 0xf1, 0xf1, 0xe6, 0x00,
  0x42, 0x42, 0x40, 0xcc, 0xcc, 0x63, 0x31, 0x86, 0x60, 0x66, 0x6e, 0x31, 0x98, 0xc3, 0x36, 0x00,
  0x42, 0x42, 0x40, 0xcc, 0xcc, 0x63, 0x31, 0x86, 0x60, 0x7c, 0x70, 0x31, 0x98, 0xc3, 0x06, 0x00,
  0x42, 0x42, 0x40, 0xcc, 0xfc, 0x63, 0xf1, 0x87, 0xe0, 0x60, 0x60, 0x31, 0x98, 0xc1, 0xe6, 0x00,
  0x42, 0x42, 0x40, 0xcc, 0xc0, 0x63, 0x01, 0x86, 0x00, 0x60, 0x60, 0x31, 0x98, 0xc0, 0x36, 0x00,
  0x42, 0x42, 0x40, 0xcc, 0xcc, 0x63, 0x31, 0x86, 0x60, 0x60, 0x60, 0x31, 0x98, 0xc3, 0x36, 0x00,
  0x42, 0x42, 0x40, 0xf8, 0x78, 0x31, 0xe0, 0xe3, 0xc0, 0x60, 0x60, 0x31, 0x98, 0x71, 0xe6, 0x00,
  0x42, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x41, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x41, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x1c, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0xc3, 0x80, 0x0e, 0x00, 0x00, 0x80, 0x00, 0x02, 0x08, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xc0, 0x11, 0x00, 0x20, 0x00, 0x00, 0x02, 0x08, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xe0, 0x11, 0x1c, 0x78, 0x87, 0x1e, 0x03, 0x18, 0xe1, 0xe3, 0x80, 0x00, 0x00, 0x00,
  0x0f, 0xff, 0xf0, 0x11, 0x22, 0x20, 0x88, 0x91, 0x02, 0xa9, 0x12, 0x24, 0x40, 0x00, 0x00, 0x00,
  0x0f, 0xff, 0xf0, 0x11, 0x20, 0x20, 0x88, 0x91, 0x02, 0x49, 0x12, 0x24, 0x40, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xe0, 0x1f, 0x20, 0x20, 0x88, 0x91, 0x02, 0x09, 0x12, 0x27, 0xc0, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xc0, 0x11, 0x20, 0x20, 0x88, 0x91, 0x02, 0x09, 0x12, 0x24, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x03, 0x80, 0x11, 0x22, 0x20, 0x88, 0x91, 0x02, 0x09, 0x12, 0x24, 0x40, 0x00, 0x00, 0x00,
  0x00, 0x07, 0x00, 0x11, 0x1c, 0x18, 0x87, 0x11, 0x02, 0x08, 0xe1, 0xe3, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x07, 0x9c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t add_prints_img[1024] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x1c, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x07, 0x00, 0x0e, 0x00, 0x00, 0x80, 0x00, 0x02, 0x08, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
  0x03, 0xc3, 0x80, 0x11, 0x00, 0x20, 0x00, 0x00, 0x02, 0x08, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xc0, 0x11, 0x1c, 0x78, 0x87, 0x1e, 0x03, 0x18, 0xe1, 0xe3, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xe0, 0x11, 0x22, 0x20, 0x88, 0x91, 0x02, 0xa9, 0x12, 0x24, 0x40, 0x00, 0x00, 0x00,
  0x0f, 0xff, 0xf0, 0x11, 0x20, 0x20, 0x88, 0x91, 0x02, 0x49, 0x12, 0x24, 0x40, 0x00, 0x00, 0x00,
  0x0f, 0xff, 0xf0, 0x1f, 0x20, 0x20, 0x88, 0x91, 0x02, 0x09, 0x12, 0x27, 0xc0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xe0, 0x11, 0x20, 0x20, 0x88, 0x91, 0x02, 0x09, 0x12, 0x24, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xc0, 0x11, 0x22, 0x20, 0x88, 0x91, 0x02, 0x09, 0x12, 0x24, 0x40, 0x00, 0x00, 0x00,
  0x00, 0x03, 0x80, 0x11, 0x1c, 0x18, 0x87, 0x11, 0x02, 0x08, 0xe1, 0xe3, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x07, 0x9c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00,
  0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00,
  0x40, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x41, 0x81, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x42, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x44, 0x18, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x44, 0x18, 0x20, 0x0f, 0x01, 0x81, 0x81, 0xf0, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x48, 0x18, 0x10, 0x19, 0x81, 0x81, 0x81, 0x98, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x48, 0x18, 0x10, 0x19, 0x8f, 0x8f, 0x81, 0x99, 0x98, 0xc7, 0xc7, 0xc7, 0x80, 0x00, 0x06, 0x00,
  0x49, 0xff, 0x90, 0x19, 0x99, 0x99, 0x81, 0x99, 0xb8, 0xc6, 0x63, 0x0c, 0xc0, 0x00, 0x06, 0x00,
  0x49, 0xff, 0x90, 0x19, 0x99, 0x99, 0x81, 0xf1, 0xc0, 0xc6, 0x63, 0x0c, 0x00, 0x00, 0x06, 0x00,
  0x48, 0x18, 0x10, 0x1f, 0x99, 0x99, 0x81, 0x81, 0x80, 0xc6, 0x63, 0x07, 0x80, 0x00, 0x06, 0x00,
  0x48, 0x18, 0x10, 0x19, 0x99, 0x99, 0x81, 0x81, 0x80, 0xc6, 0x63, 0x00, 0xc0, 0x00, 0x06, 0x00,
  0x44, 0x18, 0x20, 0x19, 0x99, 0x99, 0x81, 0x81, 0x80, 0xc6, 0x63, 0x0c, 0xc0, 0x00, 0x06, 0x00,
  0x44, 0x18, 0x20, 0x19, 0x8f, 0x8f, 0x81, 0x81, 0x80, 0xc6, 0x61, 0xc7, 0x80, 0x00, 0x06, 0x00,
  0x42, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x41, 0x81, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x40, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x20, 0x1e, 0x00, 0x20, 0x00, 0x00, 0x03, 0xc0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x07, 0xff, 0xe0, 0x11, 0x00, 0x20, 0x02, 0x00, 0x02, 0x20, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x11, 0x1c, 0x21, 0xc7, 0x8e, 0x02, 0x24, 0xc4, 0x78, 0xf1, 0xc0, 0x00, 0x00,
  0x02, 0x42, 0x40, 0x11, 0x22, 0x22, 0x22, 0x11, 0x02, 0x25, 0x04, 0x44, 0x42, 0x20, 0x00, 0x00,
  0x02, 0x42, 0x40, 0x11, 0x22, 0x22, 0x22, 0x11, 0x03, 0xc6, 0x04, 0x44, 0x42, 0x00, 0x00, 0x00,
  0x02, 0x42, 0x40, 0x11, 0x3e, 0x23, 0xe2, 0x1f, 0x02, 0x04, 0x04, 0x44, 0x41, 0xc0, 0x00, 0x00,
  0x02, 0x42, 0x40, 0x11, 0x20, 0x22, 0x02, 0x10, 0x02, 0x04, 0x04, 0x44, 0x40, 0x20, 0x00, 0x00,
  0x02, 0x42, 0x40, 0x11, 0x22, 0x22, 0x22, 0x11, 0x02, 0x04, 0x04, 0x44, 0x42, 0x20, 0x00, 0x00,
  0x02, 0x42, 0x40, 0x1e, 0x1c, 0x11, 0xc1, 0x8e, 0x02, 0x04, 0x04, 0x44, 0x31, 0xc0, 0x00, 0x00,
  0x02, 0x42, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t action_mode_img[1024] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x07, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x1e, 0x00, 0x20, 0x00, 0x00, 0x03, 0xc0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x42, 0x40, 0x11, 0x00, 0x20, 0x02, 0x00, 0x02, 0x20, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
  0x02, 0x42, 0x40, 0x11, 0x1c, 0x21, 0xc7, 0x8e, 0x02, 0x24, 0xc4, 0x78, 0xf1, 0xc0, 0x00, 0x00,
  0x02, 0x42, 0x40, 0x11, 0x22, 0x22, 0x22, 0x11, 0x02, 0x25, 0x04, 0x44, 0x42, 0x20, 0x00, 0x00,
  0x02, 0x42, 0x40, 0x11, 0x22, 0x22, 0x22, 0x11, 0x03, 0xc6, 0x04, 0x44, 0x42, 0x00, 0x00, 0x00,
  0x02, 0x42, 0x40, 0x11, 0x3e, 0x23, 0xe2, 0x1f, 0x02, 0x04, 0x04, 0x44, 0x41, 0xc0, 0x00, 0x00,
  0x02, 0x42, 0x40, 0x11, 0x20, 0x22, 0x02, 0x10, 0x02, 0x04, 0x04, 0x44, 0x40, 0x20, 0x00, 0x00,
  0x02, 0x42, 0x40, 0x11, 0x22, 0x22, 0x22, 0x11, 0x02, 0x04, 0x04, 0x44, 0x42, 0x20, 0x00, 0x00,
  0x02, 0x00, 0x40, 0x1e, 0x1c, 0x11, 0xc1, 0x8e, 0x02, 0x04, 0x04, 0x44, 0x31, 0xc0, 0x00, 0x00,
  0x01, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00,
  0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00,
  0x4e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x40, 0x1c, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x40, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x40, 0x07, 0x00, 0x0f, 0x00, 0x00, 0x18, 0x00, 0x00, 0x0c, 0x60, 0x00, 0x60, 0x00, 0x06, 0x00,
  0x43, 0xc3, 0x80, 0x19, 0x80, 0x0c, 0x00, 0x00, 0x00, 0x0c, 0x60, 0x00, 0x60, 0x00, 0x06, 0x00,
  0x40, 0x01, 0xc0, 0x19, 0x8f, 0x1f, 0x18, 0x78, 0xf8, 0x0e, 0xe3, 0xc3, 0xe3, 0xc0, 0x06, 0x00,
  0x40, 0x00, 0xe0, 0x19, 0x99, 0x8c, 0x18, 0xcc, 0xcc, 0x0f, 0xe6, 0x66, 0x66, 0x60, 0x06, 0x00,
  0x4f, 0xff, 0xf0, 0x19, 0x98, 0x0c, 0x18, 0xcc, 0xcc, 0x0d, 0x66, 0x66, 0x66, 0x60, 0x06, 0x00,
  0x4f, 0xff, 0xf0, 0x1f, 0x98, 0x0c, 0x18, 0xcc, 0xcc, 0x0c, 0x66, 0x66, 0x67, 0xe0, 0x06, 0x00,
  0x40, 0x00, 0xe0, 0x19, 0x98, 0x0c, 0x18, 0xcc, 0xcc, 0x0c, 0x66, 0x66, 0x66, 0x00, 0x06, 0x00,
  0x40, 0x01, 0xc0, 0x19, 0x99, 0x8c, 0x18, 0xcc, 0xcc, 0x0c, 0x66, 0x66, 0x66, 0x60, 0x06, 0x00,
  0x40, 0x03, 0x80, 0x19, 0x8f, 0x07, 0x18, 0x78, 0xcc, 0x0c, 0x63, 0xc3, 0xe3, 0xc0, 0x06, 0x00,
  0x40, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x40, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x47, 0x9c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x40, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x81, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x04, 0x18, 0x20, 0x0e, 0x02, 0x04, 0x0f, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x04, 0x18, 0x20, 0x11, 0x02, 0x04, 0x08, 0x80, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x18, 0x10, 0x11, 0x1e, 0x3c, 0x08, 0x93, 0x11, 0xe3, 0xc7, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x18, 0x10, 0x11, 0x22, 0x44, 0x08, 0x94, 0x11, 0x11, 0x08, 0x80, 0x00, 0x00, 0x00, 0x00,
  0x09, 0xff, 0x90, 0x11, 0x22, 0x44, 0x0f, 0x18, 0x11, 0x11, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x09, 0xff, 0x90, 0x1f, 0x22, 0x44, 0x08, 0x10, 0x11, 0x11, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x18, 0x10, 0x11, 0x22, 0x44, 0x08, 0x10, 0x11, 0x11, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x18, 0x10, 0x11, 0x22, 0x44, 0x08, 0x10, 0x11, 0x11, 0x08, 0x80, 0x00, 0x00, 0x00, 0x00,
  0x04, 0x18, 0x20, 0x11, 0x1e, 0x3c, 0x08, 0x10, 0x11, 0x10, 0xc7, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x04, 0x18, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x81, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const int okpin = 5; 
const int uppin = 18;
const int downpin = 19;  
void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x"));
  Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x"));
  Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: "));
  Serial.println(finger.capacity);
  Serial.print(F("Security level: "));
  Serial.println(finger.security_level);
  Serial.print(F("Device address: "));
  Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: "));
  Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: "));
  Serial.println(finger.baud_rate);

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  } else {
    Serial.println("Waiting for valid finger...");
    Serial.print("Sensor contains ");
    Serial.print(finger.templateCount);
    Serial.println(" templates");
  }


  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  display.clearDisplay();
  pinMode(okpin, INPUT_PULLUP);
  pinMode(uppin, INPUT_PULLUP);
  pinMode(downpin, INPUT_PULLUP);
   display.drawBitmap(0, 0, action_mode_img, 128, 64, 1);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()  // run over and over again
{
  if (digitalRead(uppin) == LOW && up_clicked == 0 && screen == 0) {
    screen = 1;
    up_clicked = 1;
    display.clearDisplay();
    display.drawBitmap(0, 0, action_mode_img, 128, 64, 1);
    display.display();
  }
  if (digitalRead(uppin) == LOW && up_clicked == 0 && screen == 1) {
    screen = 2;
    up_clicked = 1;
    display.clearDisplay();
    display.drawBitmap(0, 0, delete_prints_img, 128, 64, 1);
    display.display();
  }
  if (digitalRead(uppin) == LOW && up_clicked == 0 && screen == 2) {
    screen = 0;
    up_clicked = 1;
    display.clearDisplay();
    display.drawBitmap(0, 0, add_prints_img, 128, 64, 1);
    display.display();
  }


  if ((digitalRead(downpin) == LOW) && down_clicked == 0 && screen == 2) {
    down_clicked = 1;
    screen = 1;

    display.clearDisplay();
    display.drawBitmap(0, 0, action_mode_img, 128, 64, 1);
    display.display();
  }
  if ((digitalRead(downpin) == LOW) && down_clicked == 0 && screen == 0) {
    screen = 2;
    down_clicked = 1;
    display.clearDisplay();
    display.drawBitmap(0, 0, delete_prints_img, 128, 64, 1);
    display.display();
  }
  if ((digitalRead(downpin) == LOW) && down_clicked == 0 && screen == 1) {
    screen = 0;
    down_clicked = 1;
    display.clearDisplay();
    display.drawBitmap(0, 0, add_prints_img, 128, 64, 1);
    display.display();
  }
  if ((digitalRead(uppin) == HIGH) && up_clicked == 1) {

    up_clicked = 0;
  }
  if ((digitalRead(downpin) == HIGH) && down_clicked == 1) {

    down_clicked = 0;
  }
  if ((digitalRead(okpin) == LOW) && ok_clicked == 0 && screen == 0) {
    id = readnumber();
    if (id == 0) {  // ID #0 not allowed, try again!
      return;
    }
    Serial.print("Enrolling ID #");
    while (!getFingerprintEnroll(id))
      ;
    ok_clicked = 1;
    screen = 1;
    
    display.clearDisplay();
    display.drawBitmap(0, 0, action_mode_img, 128, 64, 1);
    display.display();
  }
  if ((digitalRead(okpin) == LOW) && ok_clicked == 0 && screen == 1) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      fcount = 0;
      end=1;
      while (end == 1) {
        display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("Place finger for sensor for attendence");
    display.display();
        y = getFingerprintID();


        if (y == 1) {
          end = 0;
          Serial.println(datalink);

          String serverPath = datalink;

          http.begin(serverPath.c_str());

          printf("routine 1 triggered /n");
          int httpResponseCode = http.GET();
          http.end();
        }
        delay(50);
      }
    }
    ok_clicked = 1;
    screen = 1;
    display.clearDisplay();
    display.drawBitmap(0, 0, action_mode_img, 128, 64, 1);
    display.display();
  }
  if ((digitalRead(okpin) == LOW) && ok_clicked == 0 && screen == 2) {
    uint8_t id = readnumber();
    if (id == 0) {  // ID #0 not allowed, try again!
      return;
    }
    deleteFingerprint(id);
    delay(2000);
    ok_clicked = 1;
    screen = 1;

    display.clearDisplay();
    display.drawBitmap(0, 0, action_mode_img, 128, 64, 1);
    display.display();
  }
  if ((digitalRead(okpin) == HIGH) && ok_clicked == 1) {

    ok_clicked = 0;
  }  //don't ned to run this at full speed.
}
////////////////////////////////////////////////////
uint8_t readnumber(void) {
  uint8_t id = 0;
  int x = 0;
  Serial.println("Please type in the ID # (from 1 to 127)...");
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("ENTER ID NUMBER");
  display.display();
  delay(2000);
  while (x == 0) {

    if ((digitalRead(downpin) == LOW) && up_clicked == 0) {
      id = id + 1;
      up_clicked = 1;
    }
    if ((digitalRead(uppin) == LOW) && down_clicked == 0) {
      id = id - 1;
      down_clicked = 1;
    }
    if ((digitalRead(downpin) == HIGH) && up_clicked == 1) {

      up_clicked = 0;
    }
    if ((digitalRead(uppin) == HIGH) && down_clicked == 1) {

      down_clicked = 0;
    }
    if (digitalRead(okpin) == LOW) {
      x = 1;
    }
    Serial.println(id);
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println(id);
    display.display();
  }
  delay(50);
  return id;
}

/////////////////////////////////////////////////////////////////////////////////
uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);
  int x = finger.fingerID;
  display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("id:");
    display.print(x);
    display.display();

  datalink = datalink + "&" + x + "=1";
  Serial.println(datalink);



  if (x == 1) {
    datalink = datalink + "1=1";
  }

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) return -1;

  // found a match!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);
  return finger.fingerID;
}


//////////////////////////////////////////////////////////////////////////////
uint8_t getFingerprintEnroll(uint8_t id) {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #");
  Serial.println(id);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("PLACE YOUR FINGER ON SENSOR");
  display.display();
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 10);
        display.println("IMAGE TAKEN");
        display.display();
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");

      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("REMOVE FINGER");
  display.display();
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID ");
  Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("PLACE THE SAME FINGER");
  display.display();
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 10);
        display.println("IMAGE TAKEN");
        display.display();
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");
  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID ");
  Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("FINGERPRINT STORED SUCCESSFULLY");
    display.display();
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }
  delay(2000);

  return true;
}
///////////////////////////////////////////////////////////////
uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;

  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("ID DELETED");
    display.display();
    Serial.println(id);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("COMMUNICATION ERROR");
    display.display();
    Serial.println(id);
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("CANT DELETE THAT");
    display.display();
    Serial.println(id);
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("ERROR WRITING TO FLASH");
    display.display();
    Serial.println(id);
  } else {
    Serial.print("Unknown error: 0x");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("UNKNOWN ERROR");
    display.display();
    Serial.println(id);
    Serial.println(p, HEX);
  }
  delay(2000);
  return p;
}
