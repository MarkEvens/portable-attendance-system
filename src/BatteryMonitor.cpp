#include "BatteryMonitor.h"
#include <esp_log.h>
#include "OledDisplay.h"

// Logging tag
static const char *TAG = "BatteryMonitor";
int old_percentage = 100;
// Constructor for BatteryMonitor class
BatteryMonitor::BatteryMonitor(OledDisplay &display)
  : oledDisplay(display) {}

// Initialize the ADC
void BatteryMonitor::init() {
  initADC();
  ESP_LOGI(TAG, "BatteryMonitor initialized");
}

// Initialize the ADC for battery voltage measurement
void BatteryMonitor::initADC() {
  adc1_config_width(ADC_WIDTH_BIT_12);                  // 12-bit resolution
  adc1_config_channel_atten(ADC_PIN, ADC_ATTEN_DB_11);  // 0-3.9V range
  ESP_LOGI(TAG, "ADC initialized");
}

// Read battery voltage
float BatteryMonitor::readBatteryVoltage() {
  int adcValue = adc1_get_raw(ADC_PIN);
  float voltage = ((float)adcValue / 4095.0) * 3.3 * 2;  // Adjust for voltage divider
  ESP_LOGI(TAG, "Battery Voltage: %.2fV", voltage);
  return voltage;
}

// Calculate battery percentage based on voltage
int BatteryMonitor::calculateBatteryPercentage(float voltage) {
  if (voltage >= MAX_BATTERY_VOLTAGE) {
    return 100;
  } else if (voltage <= MIN_BATTERY_VOLTAGE) {
    return 0;
  } else {
    return (int)((voltage - MIN_BATTERY_VOLTAGE) / (MAX_BATTERY_VOLTAGE - MIN_BATTERY_VOLTAGE) * 100);
  }
}

// Update the OLED display with battery percentage
void BatteryMonitor::show(){
  float voltage = readBatteryVoltage();
  int new_percentage = calculateBatteryPercentage(voltage);
   char buffer[16];
    snprintf(buffer, sizeof(buffer), "%d%%", new_percentage);
    oledDisplay.update_battery(buffer);
}
void BatteryMonitor::update() {
 
  float voltage = readBatteryVoltage();
  int new_percentage = calculateBatteryPercentage(voltage);

  if (old_percentage != new_percentage) {
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%d%%", new_percentage);
    oledDisplay.update_battery(buffer);
    old_percentage = new_percentage;
  }
  // Display percentage on OLED
}
