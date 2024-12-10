#include "Attendencemanager.h"
#include "FingerPrint.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "OledDisplay.h"
extern OledDisplay oledDisplay; // Declare oledDisplay as external, so it's not redefined here
#define MAX_ENTRIES 1000
int numbers[MAX_ENTRIES]; // Array to store enrolled numbers
int count = 0;            // Number of enrolled fingerprints

// Initialize NVS
void init_nvs()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS partition was truncated and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

// Function to load enrolled numbers from NVS
void load_enrolled_numbers()
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (err == ESP_OK)
    {
        size_t required_size = sizeof(numbers); // Number of bytes for the array
        err = nvs_get_blob(nvs_handle, "enrolled_numbers", numbers, &required_size);
        if (err == ESP_OK)
        {
            count = required_size / sizeof(int); // Update count based on stored data
            Serial.printf("[INFO] Loaded %d enrolled numbers from NVS.\n", count);
        }
        else
        {
            Serial.println("[ERROR] Failed to load enrolled numbers from NVS.");
        }
        nvs_close(nvs_handle);
    }
}

// Function to save enrolled numbers to NVS
void save_enrolled_numbers()
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (err == ESP_OK)
    {
        err = nvs_set_blob(nvs_handle, "enrolled_numbers", numbers, count * sizeof(int));
        if (err == ESP_OK)
        {
            nvs_commit(nvs_handle); // Save the changes to flash
            Serial.println("[INFO] Enrolled numbers saved to NVS.");
        }
        else
        {
            Serial.println("[ERROR] Failed to save enrolled numbers to NVS.");
        }
        nvs_close(nvs_handle);
    }
}

void attendencemanager_add(int num)
{
    if (num < 0 || num >= MAX_ENTRIES)
    {
        Serial.printf("[ERROR] Invalid number! Only numbers between 0 and %d are allowed.\n", MAX_ENTRIES);
        return;
    }

    if (count >= MAX_ENTRIES)
    {
        Serial.println("[ERROR] Enrollment list is full!");
        return;
    }

    if (attendencemanager_check(num))
    {
        Serial.printf("[INFO] Number %d is already enrolled.\n", num);
    }
    else
    {
        numbers[count++] = num; // Add the number to the list
        Fingerprint_add(num);   // Simulate adding to the fingerprint module
        Serial.printf("[INFO] Number %d added successfully.\n", num);
        // Save updated list to NVS
    }
}
int attendencemanager_auth()
{

    int num = Fingerprint_scan();

    if (num == 0)
    {
        return 1; // Number is found
        Serial.println("authenticated Successfully");
        oledDisplay.displayText("authenticated Successfully");
    }
    if(num!=0)
    {
        
        
        return 0; // Number is not found
    }

    return 0; // Number is not found
}
int attendencemanager_check(int num)
{
    for (int i = 0; i < count; i++)
    {
        if (numbers[i] == num)
        {
            return 1; // Number is found
        }
    }
    return 0; // Number is not found
}

// Function to add a number to the enrollment

// Function to remove a number from the enrollment
void attendencemanager_remove(int num)
{
    if (num < 0 || num > MAX_ENTRIES)
    {
        printf("Invalid number! Only positive numbers less than or equal to %d are allowed.\n", MAX_ENTRIES);
        oledDisplay.displayText("Invalid number");

        return;
    }

    int found = 0;
    for (int i = 0; i < count; i++)
    {
        if (numbers[i] == num)
        {
            found = 1;
            for (int j = i; j < count - 1; j++)
            {
                numbers[j] = numbers[j + 1]; // Shift the remaining numbers
            }
            count--;
            printf("Number %d removed successfully.\n", num);
            Fingerprint_delete(num);
            break;
        }
    }
    if (!found)
    {
        printf("Number %d not found in the enrollment list.\n", num);
        oledDisplay.displayText("Number not found");
    }
}
