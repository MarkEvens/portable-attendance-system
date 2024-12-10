#include "BatteryMonitor.h"
#include "OledDisplay.h"
#include "ButtonAttendence.h"
#include "StateMachine.h"
#include "FingerPrint.h"
#include "AttendenceWifi.h"
#include "Attendencemanager.h"
#include <HTTPClient.h>
#include <WiFi.h>
#include <Arduino.h>
#include "SleepManager.h"

// Constants
const uint32_t SLEEP_DELAY_MS = 10 * 60 * 1000; // 10 minutes
const gpio_num_t WAKEUP_PIN = GPIO_NUM_0;       // Pin for the hardware interrupt

// Create a SleepManager instance
SleepManager sleepManager(SLEEP_DELAY_MS, WAKEUP_PIN);
OledDisplay oledDisplay;
BatteryMonitor batteryMonitor(oledDisplay);

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);

    // Initialize the display, battery monitor, and fingerprint module
    oledDisplay.init();
    batteryMonitor.init();
    button_attendence_init();
    oledDisplay.scanThumbInit();
    Fingerprint_init();

    // Set up Wi-Fi
    WIFI_setup();
    sleepManager.begin();
}

void loop()
{
    if (getMenuState() == IDLE)
    {
        if (getattendencemode() == ON)
        {
            oledDisplay.scanThumbInit();
            int enno = Fingerprint_scan(); // Get enrollment number
            delay(200);

            // Only if valid enrollment number
            if (enno == 0)
            {
                String serverPath = getDatalink(); // Get URL from `getDatalink()`
                Serial.println("Attendance State, requesting URL: " + serverPath);

                if (WiFi.status() == WL_CONNECTED) // Check if Wi-Fi is connected
                {
                    HTTPClient http;        // Create a new HTTPClient object
                    http.begin(serverPath); // Pass the URL string

                    int httpResponseCode = http.GET(); // Send GET request

                    if (httpResponseCode > 0)
                    {
                        // Print response
                        Serial.printf("HTTP Response code: %d\n", httpResponseCode);
                        String response = http.getString(); // Get the response as a String
                        Serial.println(response);
                        url_resetter(); // Print the response
                    }
                    else
                    {
                        // If the request failed, print the error code
                        Serial.printf("Error in HTTP request, code: %d\n", httpResponseCode);
                    }

                    http.end(); // Close connection
                }
                else
                {
                    // If Wi-Fi is not connected, print an error message
                    Serial.println("[ERROR] Wi-Fi not connected");
                }
            }
            else
            {
                Serial.println("[ERROR] Invalid fingerprint scan");
            }
        }
    }
}
