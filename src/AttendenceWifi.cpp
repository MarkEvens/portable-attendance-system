#include "WiFiProv.h"
#include "WiFi.h"

// #define USE_SOFT_AP // Uncomment if you want to enforce using the Soft AP method instead of BLE
const char *pop = "abcd1234";           // Proof of possession - otherwise called a PIN - string provided by the device, entered by the user in the phone app
const char *service_name = "PROV_123";  // Name of your device (the Espressif apps expects by default device name starting with "Prov_")
const char *service_key = NULL;         // Password used for SoftAP method (NULL = no password needed)
bool reset_provisioned = true;          // When true the library will automatically delete previously provisioned data.

// WARNING: SysProvEvent is called from a separate FreeRTOS task (thread)!
void SysProvEvent(arduino_event_t *sys_event) {
  switch (sys_event->event_id) {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.print("\nConnected IP address : ");
      Serial.println(IPAddress(sys_event->event_info.got_ip.ip_info.ip.addr));
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED: Serial.println("\nDisconnected. Connecting to the AP again... "); break;
    case ARDUINO_EVENT_PROV_START:            Serial.println("\nProvisioning started\nGive Credentials of your access point using smartphone app"); break;
    case ARDUINO_EVENT_PROV_CRED_RECV:
    {
      Serial.println("\nReceived Wi-Fi credentials");
      Serial.print("\tSSID : ");
      Serial.println((const char *)sys_event->event_info.prov_cred_recv.ssid);
      Serial.print("\tPassword : ");
      Serial.println((char const *)sys_event->event_info.prov_cred_recv.password);
      break;
    }
    case ARDUINO_EVENT_PROV_CRED_FAIL:
    {
      Serial.println("\nProvisioning failed!\nPlease reset to factory and retry provisioning\n");
      if (sys_event->event_info.prov_fail_reason == WIFI_PROV_STA_AUTH_ERROR) {  // Updated constant
        Serial.println("\nWi-Fi AP password incorrect");
      } else {
        Serial.println("\nWi-Fi AP not found....Add API \"nvs_flash_erase()\" before beginProvision()");
      }
      break;
    }
    case ARDUINO_EVENT_PROV_CRED_SUCCESS: Serial.println("\nProvisioning Successful"); break;
    case ARDUINO_EVENT_PROV_END:          Serial.println("\nProvisioning Ends"); break;
    default:                              break;
  }
}

void WIFI_setup() {
  WiFi.begin();  // no SSID/PWD - get it from the Provisioning APP or from NVS (last successful connection)
  WiFi.onEvent(SysProvEvent);

// BLE Provisioning using the ESP SoftAP Prov works fine for any BLE SoC, including ESP32, ESP32S3, and ESP32C3.
#if CONFIG_BLUEDROID_ENABLED && !defined(USE_SOFT_AP)
  Serial.println("Begin Provisioning using BLE");
  // Sample uuid that user can pass during provisioning using BLE
  uint8_t uuid[16] = {0xb4, 0xdf, 0x5a, 0x1c, 0x3f, 0x6b, 0xf4, 0xbf, 0xea, 0x4a, 0x82, 0x03, 0x04, 0x90, 0x1a, 0x02};
  WiFiProv.beginProvision(
    WIFI_PROV_SCHEME_BLE,                      // Updated constant
    WIFI_PROV_SCHEME_HANDLER_FREE_BLE,         // Updated constant
    WIFI_PROV_SECURITY_1,                      // Updated constant
    pop, service_name, service_key, uuid, reset_provisioned
  );
  log_d("BLE QR");
  WiFiProv.printQR(service_name, pop, "ble");
#else
  Serial.println("Begin Provisioning using Soft AP");
  WiFiProv.beginProvision(
    WIFI_PROV_SCHEME_SOFTAP,                   // Updated constant
    WIFI_PROV_SCHEME_HANDLER_NONE,             // Updated constant
    WIFI_PROV_SECURITY_1,                      // Updated constant
    pop, service_name, service_key
  );
  log_d("WiFi QR");
  WiFiProv.printQR(service_name, pop, "softap");
#endif
}
bool isWiFiConnected() {
  // Check if the WiFi is connected to an access point
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("[INFO] Wi-Fi is connected.");
    return true;
  } else {
    Serial.println("[ERROR] Wi-Fi is not connected.");
    return false;
  }
}
