#ifndef ATTENDENCEWIFI_H
#define ATTENDENCEWIFI_H
#include "WiFiProv.h"
#include "WiFi.h"


void WIFI_setup();
void SysProvEvent(arduino_event_t *sys_event);
bool isWiFiConnected();
#endif