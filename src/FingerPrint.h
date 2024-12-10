#ifndef FINGERPRINT_H
#define FINGERPRINT_H   

// for esp32
// #define RXD2 16
// #define TXD2 17

#define RXD2 44
#define TXD2 43


#include <Adafruit_Fingerprint.h>
#include "OledDisplay.h"
#include "ButtonAttendence.h"   
#include <Adafruit_Fingerprint.h>
String getDatalink(); 
void Fingerprint_init();
uint8_t Fingerprint_scan();
uint8_t Fingerprint_delete(uint8_t id);
uint8_t Fingerprint_add(uint8_t id);


void url_resetter();


#endif // FINGERPRINT_H