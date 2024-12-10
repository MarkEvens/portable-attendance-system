#include "FingerPrint.h"
#include "OledDisplay.h"
#include "ButtonAttendence.h"
#include "OledDisplay.h"
#include "ButtonAttendence.h"
#include "StateMachine.h"
// UART2 on ESP32
extern OledDisplay oledDisplay; // Declare oledDisplay as external, so it's not redefined here
String datalink = "https://script.google.com/macros/s/AKfycbwKwYUNivEx_G3stjE3VbiL5cMjUJotp5Mo8fAKRAdsj49Jc0H4G5-yJ76EFZ94cHI/exec?";
String datalink2 = "https://script.google.com/macros/s/AKfycbwKwYUNivEx_G3stjE3VbiL5cMjUJotp5Mo8fAKRAdsj49Jc0H4G5-yJ76EFZ94cHI/exec?";

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);
void url_resetter()
{
  datalink = datalink2;
}
void Fingerprint_init()
{
  Serial2.begin(57600, SERIAL_8N1, RXD2, TXD2);
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword())
  {
    Serial.println("Found fingerprint sensor!");
  }
  else
  {
    Serial.println("Did not find fingerprint sensor :(");
    while (1)
    {
      delay(1);
    }
  }

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

  if (finger.templateCount == 0)
  {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else
  {
    Serial.println("Waiting for valid finger...");
    Serial.print("Sensor contains ");
    Serial.print(finger.templateCount);
    Serial.println(" templates");
  }
}

uint8_t Fingerprint_scan()
{
  uint8_t p = finger.getImage();
  switch (p)
  {
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
  switch (p)
  {
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
    oledDisplay.displayText("attendence fail try again");
    return p;
  case FINGERPRINT_INVALIDIMAGE:
    Serial.println("Could not find fingerprint features");
    oledDisplay.displayText("attendence fail try again");

    return p;
  default:
    Serial.println("Unknown error");
    oledDisplay.displayText("attendence fail try again");

    return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK)
  {
    Serial.println("Found a print match!");
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR)
  {
    Serial.println("Communication error");
    return p;
  }
  else if (p == FINGERPRINT_NOTFOUND)
  {
    Serial.println("Did not find a match");
    if (getMenuState() == IDLE)
    {
      if (getattendencemode() == ON)
      {
        Serial.println("authentication failed");
        oledDisplay.authFailedInit();
      }
    }
    return p;
  }
  else
  {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  int fingernum = finger.fingerID;
  oledDisplay.displayNumber(fingernum);
  delay(1000);
  Serial.println(finger.confidence);
  if (getMenuState() == IDLE)
  {
    if (getattendencemode() == ON)
    {
      datalink = datalink + "&" + fingernum + "=1";

      if (fingernum == 0)
      {
        datalink = datalink + "&1=1";
      }
    }
  }

  return fingernum;
}
String getDatalink()
{

  return datalink;
}

uint8_t Fingerprint_delete(uint8_t id)
{
  uint8_t p = -1;

  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK)
  {
    Serial.println("Deleted!");
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR)
  {
    Serial.println("Communication error");
  }
  else if (p == FINGERPRINT_BADLOCATION)
  {
    Serial.println("Could not delete in that location");
  }
  else if (p == FINGERPRINT_FLASHERR)
  {
    Serial.println("Error writing to flash");
  }
  else
  {
    Serial.print("Unknown error: 0x");
    Serial.println(p, HEX);
  }

  return p;
}

uint8_t Fingerprint_add(uint8_t id)
{

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #");
  oledDisplay.displayText("Waiting for valid finger to enroll as #");
  Serial.println(id);

  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
    switch (p)
    {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      oledDisplay.displayText("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      oledDisplay.displayText("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      oledDisplay.displayText("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      oledDisplay.displayText("Unknown error");
      break;
    }
  }

  // OK success!
  p = finger.image2Tz(1);
  switch (p)
  {
  case FINGERPRINT_OK:
    Serial.println("Image converted");
    oledDisplay.displayText("Image converted");
    break;
  case FINGERPRINT_IMAGEMESS:
    Serial.println("Image too messy");
    oledDisplay.displayText("Image too messy");
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    oledDisplay.displayText("Communication error");
    return p;
  case FINGERPRINT_FEATUREFAIL:
    Serial.println("Could not find fingerprint features");
    oledDisplay.displayText("Could not find fingerprint features");
    delay(1000);
    return p;
  case FINGERPRINT_INVALIDIMAGE:
    Serial.println("Could not find fingerprint features");
    oledDisplay.displayText("Could not find fingerprint features");
    delay(1000);
    return p;
  default:
    Serial.println("Unknown error");
    oledDisplay.displayText("Unknown error");
    return p;
  }

  Serial.println("Remove finger");
  oledDisplay.displayText("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER)
  {
    p = finger.getImage();
  }
  Serial.print("ID ");
  Serial.println(id);

  p = -1;
  Serial.println("Place same finger again");
  oledDisplay.displayText("Place same finger again");
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
    switch (p)
    {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      oledDisplay.displayText("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      oledDisplay.displayText("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      oledDisplay.displayText("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      oledDisplay.displayText("Unknown error");
      break;
    }
  }

  // OK success!
  p = finger.image2Tz(2);
  switch (p)
  {
  case FINGERPRINT_OK:
    Serial.println("Image converted");
    oledDisplay.displayText("Image converted");
    break;
  case FINGERPRINT_IMAGEMESS:
    Serial.println("Image too messy");
    oledDisplay.displayText("Image too messy");
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    oledDisplay.displayText("Communication error");
    return p;
  case FINGERPRINT_FEATUREFAIL:
    Serial.println("Could not find fingerprint features");
    oledDisplay.displayText("Could not find fingerprint features");
    delay(1000);
    return p;
  case FINGERPRINT_INVALIDIMAGE:
    Serial.println("Could not find fingerprint features");
    oledDisplay.displayText("Could not find fingerprint features");
    delay(1000);
    return p;
  default:
    Serial.println("Unknown error");
    oledDisplay.displayText("Unknown error");
    return p;
  }

  // OK converted!
  Serial.print("Creating model for #");
  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK)
  {
    Serial.println("Prints matched!");
    oledDisplay.displayText("Prints matched!");
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR)
  {
    Serial.println("Communication error");
    oledDisplay.displayText("Communication error");
    return p;
  }
  else if (p == FINGERPRINT_ENROLLMISMATCH)
  {
    Serial.println("Fingerprints did not match");
    oledDisplay.displayText("Fingerprints did not match");
    delay(1000);
    return p;
  }
  else
  {
    Serial.println("Unknown error");
    oledDisplay.displayText("Unknown error");
    return p;
  }

  Serial.print("ID ");
  Serial.println(id);

  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK)
  {
    Serial.println("Stored!");
    oledDisplay.displayText("Stored!");
    delay(2000);
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR)
  {
    Serial.println("Communication error");
    oledDisplay.displayText("Communication error");
    return p;
  }
  else if (p == FINGERPRINT_BADLOCATION)
  {
    Serial.println("Could not store in that location");
    oledDisplay.displayText("Could not store in that location");
    delay(2000);
    return p;
  }
  else if (p == FINGERPRINT_FLASHERR)
  {
    Serial.println("Error writing to flash");
    oledDisplay.displayText("Error writing to flash");
    delay(2000);
    return p;
  }
  else
  {
    Serial.println("Unknown error");
    oledDisplay.displayText("Unknown error");
    delay(2000);
    return p;
  }

  return true;
}
