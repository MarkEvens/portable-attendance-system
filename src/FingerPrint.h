#ifndef FINGERPRINT_H
#define FINGERPRINT_H   


#define mySerial Serial2


#include <Adafruit_Fingerprint.h>
#include "OledDisplay.h"
#include "ButtonAttendence.h"   
#include <Adafruit_Fingerprint.h>

void Fingerprint_init();
uint8_t Fingerprint_scan();
uint8_t Fingerprint_delete(uint8_t id);
uint8_t Fingerprint_add(uint8_t id);




#endif // FINGERPRINT_H