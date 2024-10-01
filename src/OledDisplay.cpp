#include "OledDisplay.h"
#include <Wire.h>
#include <esp_log.h>
#include "AnimationImage.h"

// Logging tag
static const char *TAG = "OledDisplay";

// Constructor for OledDisplay
OledDisplay::OledDisplay()
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

// Initialize the OLED display
void OledDisplay::init()
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
  {
    ESP_LOGE(TAG, "OLED initialization failed");
    for (;;)
      ; // Stay here if initialization fails
  }
  display.clearDisplay(); // Show initial screen
  ESP_LOGI(TAG, "OLED initialized");

  display.clearDisplay();
}
/**
 * @brief Displays a splash screen animation on the OLED display.
 *
 * This function iterates through an array of bitmaps, displaying each one on the
 * OLED screen in sequence. It is intended to be used as a splash screen when the
 * device is first powered on.
 *
 * @param none
 * @return none
 */
void OledDisplay::splashScreenInit()
{
  int frame = 0;
  for (int i = 0; i <= 28; i += 1)
  {
    display.clearDisplay();
    display.drawBitmap(32, 0, EYE_BLINK_ANIMATION[frame], FRAME_WIDTH, FRAME_HEIGHT, 1);
    display.display();
    frame = (frame + 1) % FRAME_COUNT;
    delay(FRAME_DELAY);
  }
}

// Clear the OLED display
void OledDisplay::clear()
{
  display.clearDisplay();
  display.display(); // Update the display after clearing
}
void OledDisplay::SCAN_LOAD_ANIMATION()
{
  int frame = 0;
  for (int i = 0; i <= 28; i += 1)
  {
    display.clearDisplay();
    display.drawBitmap(32, 0, SCAN_LOAD_FRAMES[frame], FRAME_WIDTH, FRAME_HEIGHT, 1);
    display.display();
    frame = (frame + 1) % FRAME_COUNT;
    delay(FRAME_DELAY);
  }
}
// Display text on OLED
void OledDisplay::displayText(const char *text)
{
  clear(); // Clear the display first
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 20);
  display.print(text);
  display.display(); // Update display to show text
  ESP_LOGI(TAG, "Displayed: %s", text);
}
void OledDisplay::update_battery(const char *text)
{

  // Update display to show text
  // Update every 5 seconds  // Clear the display first
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(104, 4);
  display.print(text);
  // display.display(); // Update display to show text
  ESP_LOGI(TAG, "Displayed: %s", text);
}
/**
 * @brief Clears the display and prompts the user to scan their thumb to record attendance
 *
 */
void OledDisplay::scanThumbInit()
{
  display.clearDisplay();
  wifi_icons();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(17, 22);
  display.print("Please scan your");
  display.setCursor(46, 35);
  display.print("THUMB");
  display.setCursor(2, 48);
  display.print("to Record Attendence");
  display.display();
}

/**
 * @brief Clears the display and prompts the user to scan their thumb for authorization
 *
 */
void OledDisplay::scanThumbForAuth()
{
  display.clearDisplay();
  wifi_icons();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(17, 22);
  display.print("Please scan your");
  display.setCursor(46, 35);
  display.print("THUMB");
  display.setCursor(2, 48);
  display.print("  For Authorization");
  display.display();
}
/**
 * @brief Initializes the present screen with the user type and enrollment number
 *
 * @param user_type The type of user (faculty or student)
 * @param enroll_no The enrollment number of the user
 */
void OledDisplay::presentScreenInit(USER_TYPE user_type, int enroll_no)
{
  display.clearDisplay();
  switch (user_type)
  {
  case FACULTY:
  {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(28, 4);
    display.print("Faculty");
    break;
  }
  case STUDENT:
  {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(28, 4);
    display.print("Student");
    break;
  }
  default:
  {
    break;
  }
  }
  wifi_icons();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(12, 22);
  display.print("Attendence Recorded");
  display.setCursor(32, 44);
  display.print("En.no:-");
  display.setTextSize(2);
  display.setCursor(76, 40);
  display.print(enroll_no);
  display.display();
}
/**
 * @brief Displays a success message on the OLED display after a successful operation (add/remove)
 *
 * @param op_type The type of operation (add/remove)
 * @param user_type The type of user (faculty/student)
 * @param enroll_no The enrollment number of the user
 */
void OledDisplay::SuccessfulInit(OP_TYPE op_type, USER_TYPE user_type, int enroll_no)
{
  display.clearDisplay();
  wifi_icons();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  switch (op_type)
  {
  case ADD:
  {
    switch (user_type)
    {
    case FACULTY:
    {
      display.setCursor(28, 22);
      display.print("Faculty Added");
      break;
    }
    case STUDENT:
    {
      display.setCursor(28, 22);
      display.print("Student Added");
      break;
    }
    default:
    {
      break;
    }
    }
    break; // Add this break to prevent fall-through
  }
  case REMOVE:
  {
    switch (user_type)
    {
    case FACULTY:
    {
      display.setCursor(20, 22);
      display.print("Faculty Removed");
      break;
    }
    case STUDENT:
    {
      display.setCursor(20, 22);
      display.print("Student Removed");
      break;
    }
    default:
    {
      break;
    }
    }
    break;
  }

  default:
  {
    break;
  }
  }
  display.setCursor(32, 44);
  display.print("En.no:-");
  display.setTextSize(2);
  display.setCursor(72, 40);
  display.print(enroll_no);
  display.display();
}
/**
 * @brief Removes the user from the database and displays a not available message
 *
 * @param user_type The type of user (student or faculty)
 */
void OledDisplay::removeNotAvailableInit(USER_TYPE user_type)
{
  display.clearDisplay();
  wifi_icons();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(40, 26);
  display.print("User Not");
  display.setCursor(36, 38);
  display.print("Available");
  display.display();
}

/**
 * @brief Updates the OLED display with the enrollment number and a remove button
 *
 * @param user_type The type of user (student or faculty)
 * @param enroll_no The enrollment number of the user
 */
void OledDisplay::removeUserEnter(USER_TYPE user_type, int enroll_no)
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(16, 24);
  display.print("Select a EN.no");
  display.setTextSize(2);
  // Debug: Log the input value
  ESP_LOGI(TAG, "Input enroll_no: %d", enroll_no);
  if (enroll_no >= 0 && enroll_no <= 9)
  {
    display.setCursor(58, 40);
    display.print(enroll_no);
    ESP_LOGI(TAG, "single digit: %d", enroll_no);
  }
  else if (enroll_no >= 10 && enroll_no <= 99)
  {
    display.setCursor(52, 40);
    display.print(enroll_no);
    ESP_LOGI(TAG, "double digit: %d", enroll_no);
  }
  else if (enroll_no >= 100 && enroll_no <= 999)
  {
    display.setCursor(46, 40);
    display.print(enroll_no);
    ESP_LOGI(TAG, "triple digit: %d", enroll_no);
  }
  else
  {
    display.setCursor(40, 40);
    display.print(enroll_no);
    ESP_LOGI(TAG, "quadruple digit: %d", enroll_no);
  }
  display.display(); // Update the display
  // Debug: Log which condition was met
  if (enroll_no >= 0 && enroll_no <= 9)
    ESP_LOGI(TAG, "Condition: 0-9");
  else if (enroll_no >= 10 && enroll_no <= 99)
    ESP_LOGI(TAG, "Condition: 10-99");
  else if (enroll_no >= 100 && enroll_no <= 999)
    ESP_LOGI(TAG, "Condition: 100-999");
  else
    ESP_LOGI(TAG, "Condition: none");
  display.drawBitmap(22, 41, epd_bitmap_deleteprints, 12, 12, 1);
  display.drawBitmap(92, 41, epd_bitmap_addprints, 12, 12, 1);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20, 3);
  display.print("Remove Student");
  display.display();
}

/**
 * @brief Displays a menu for adding a new user
 *
 * @param user_type The type of user (student or faculty)
 * @param enroll_no The enrollment number of the user
 */
void OledDisplay::addUserEnterNew(USER_TYPE user_type, int enroll_no)
{
  display.fillRect(0, 20, 128, 45, BLACK);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(16, 24);
  display.print("Select a EN.no");
  display.setTextSize(2);
  // Debug: Log the input value
  ESP_LOGI(TAG, "Input enroll_no: %d", enroll_no);

  if (enroll_no >= 0 && enroll_no <= 9)
  {
    display.setCursor(58, 40);
    display.print(enroll_no);
    ESP_LOGI(TAG, "single digit: %d", enroll_no);
  }
  else if (enroll_no >= 10 && enroll_no <= 99)
  {
    display.setCursor(52, 40);
    display.print(enroll_no);
    ESP_LOGI(TAG, "double digit: %d", enroll_no);
  }
  else if (enroll_no >= 100 && enroll_no <= 999)
  {
    display.setCursor(46, 40);
    display.print(enroll_no);
    ESP_LOGI(TAG, "triple digit: %d", enroll_no);
  }
  else
  {
    display.setCursor(40, 40);
    display.print(enroll_no);
    ESP_LOGI(TAG, "quadruple digit: %d", enroll_no);
  }
  display.display(); // Update the display
  // Debug: Log which condition was met
  if (enroll_no >= 0 && enroll_no <= 9)
    ESP_LOGI(TAG, "Condition: 0-9");
  else if (enroll_no >= 10 && enroll_no <= 99)
    ESP_LOGI(TAG, "Condition: 10-99");
  else if (enroll_no >= 100 && enroll_no <= 999)
    ESP_LOGI(TAG, "Condition: 100-999");
  else
    ESP_LOGI(TAG, "Condition: none");
  display.drawBitmap(22, 41, epd_bitmap_deleteprints, 12, 12, 1);
  display.drawBitmap(92, 41, epd_bitmap_addprints, 12, 12, 1);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20, 3);
  display.print("Add Faculty");
  display.display();
}

/**
 * @brief Displays an animation for an invalid user
 *
 * This function displays a sequence of frames on the OLED display, which
 * represent an invalid user. The animation consists of a series of bitmaps
 * stored in the array notauthorizedframes, which are displayed one after
 * another. The animation is followed by a message indicating that the user
 * is invalid.
 */
void OledDisplay::invalidUserInit()
{
  int frame = 0;
  for (int i = 0; i <= 28; i += 1)
  {
    display.clearDisplay();
    display.drawBitmap(32, 0, CROSS_ANIMATION[frame], FRAME_WIDTH, FRAME_HEIGHT, 1);
    display.display();
    frame = (frame + 1) % FRAME_COUNT;
    delay(FRAME_DELAY);
  }
  display.fillRoundRect(0, 20, 128, 45, 2, BLACK);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(40, 25);
  display.print("INVALID");
  display.setCursor(44, 38);
  display.print("USER!!");
  display.display();
}
/**
 * @brief Displays an animation for a user that is not authorized
 *
 * This function displays a sequence of frames on the OLED display, which
 * represent an unauthorized user. The animation consists of a series of
 * bitmaps stored in the array notauthorizedframes, which are displayed one
 * after another. The animation is followed by a message indicating that the
 * user is not authorized.
 */
void OledDisplay::authFailedInit()
{
  int frame = 0;
  for (int i = 0; i <= 28; i += 1)
  {
    display.clearDisplay();
    display.drawBitmap(32, 0, CROSS_ANIMATION[frame], FRAME_WIDTH, FRAME_HEIGHT, 1);
    display.display();
    frame = (frame + 1) % FRAME_COUNT;
    delay(FRAME_DELAY);
  }
  display.fillRoundRect(0, 20, 128, 45, 2, BLACK);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(56, 25);
  display.print("NOT");
  display.setCursor(32, 38);
  display.print("AUTHORIZED!!");
  display.display();
}

/**
 * @brief Displays the sub menu for adding a new user
 *
 * This function displays the sub menu for adding a new user on the OLED
 * display. The sub menu consists of options for selecting the type of user
 * (student or faculty), and a 'back' option to return to the main menu.
 */
void OledDisplay::initSubMenu(MAIN_MENU main_opt, SUB_MENU opt)
{
  display.clearDisplay();
  wifi_icons();

  display.drawBitmap(0, 20, epd_bitmap_Student_option, 12, 12, 1);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(17, 22);
  display.print("Student");
  display.drawBitmap(0, 34, epd_bitmap_Faculty_option, 12, 12, 1);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(17, 36);
  display.print("Faculty"); // Update every 5 seconds
  display.drawBitmap(0, 48, epd_bitmap_back_option, 12, 12, 1);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(17, 50);
  display.print("Back");
  switch (main_opt)
  {
  case ADD_MENU /* constant-expression */:
  {
    display.setCursor(30, 3);
    display.print("ADD MENU");
    display.drawRoundRect(27, 1, 52, 12, 2, WHITE);
    switch (opt)
    {
    case STUDENT_SUB_MENU:
      // Code for case 1
      display.drawRoundRect(0, 19, 126, 14, 2, WHITE);
      Serial.println("Case 1: Option 1 selected");
      break;

    case FACULTY_SUB_MENU:
      // Code for case 2
      display.drawRoundRect(0, 33, 126, 14, 2, WHITE);
      break;

    case BACK_SUB_MENU:
      // Code for case 3
      display.drawRoundRect(0, 47, 126, 14, 2, WHITE);
      break;

    default:
      // Code for an unspecified case
      Serial.println("Default: Invalid option");
      break;
    }
    break;
  }
  case REMOVE_MENU /* constant-expression */:
  {
    display.setCursor(20, 3);
    display.print("REMOVE MENU");
    display.drawRoundRect(17, 1, 70, 12, 2, WHITE);
    switch (opt)
    {
    case STUDENT_SUB_MENU:
      // Code for case 1
      display.drawRoundRect(0, 19, 126, 14, 2, WHITE);
      Serial.println("Case 1: Option 1 selected");
      break;

    case FACULTY_SUB_MENU:
      // Code for case 2
      display.drawRoundRect(0, 33, 126, 14, 2, WHITE);
      break;

    case BACK_SUB_MENU:
      // Code for case 3
      display.drawRoundRect(0, 47, 126, 14, 2, WHITE);
      break;

    default:
      // Code for an unspecified case
      Serial.println("Default: Invalid option");
      break;
    }
    break;
  }

  default:
    break;
  }
  display.display();
}

/**
 * @brief Displays the main menu for adding or deleting prints
 *
 * This function displays the main menu for adding or deleting prints on the
 * OLED display. The main menu consists of options for adding prints, deleting
 * prints, and a 'back' option to return to the sub menu.
 */
void OledDisplay::initMainMenu(MAIN_MENU opt)
{

  display.clearDisplay();
  wifi_icons();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(25, 3);
  display.print("Main Menu");
  display.drawRoundRect(23, 1, 60, 12, 2, WHITE);
  display.drawBitmap(0, 20, epd_bitmap_addprints, 12, 12, 1);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(17, 22);
  display.print("ADD USER ");
  display.drawBitmap(0, 34, epd_bitmap_deleteprints, 12, 12, 1);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(17, 36);
  display.print("REMOVE USER"); // Update every 5 seconds
  display.drawBitmap(0, 48, epd_bitmap_back_option, 12, 12, 1);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(17, 50);
  display.print("Back");
  switch (opt)
  {
  case ADD_MENU:
    // Code for case 1
    display.drawRoundRect(0, 19, 126, 14, 2, WHITE);
    Serial.println("Case 1: Option 1 selected");
    break;

  case REMOVE_MENU:
    // Code for case 2
    display.drawRoundRect(0, 33, 126, 14, 2, WHITE);
    break;

  case BACK_MENU:
    // Code for case 3
    display.drawRoundRect(0, 47, 126, 14, 2, WHITE);
    break;

  default:
    // Code for an unspecified case
    Serial.println("Default: Invalid option");
    break;
  }

  display.display();
}
/**
 * @brief Display the battery and wifi icons on the OLED display
 *
 * This function displays the battery and wifi icons on the OLED display. The
 * battery icon is displayed at the top right corner of the display, and the
 * wifi icon is displayed at the top left corner of the display.
 */
void OledDisplay::wifi_icons()
{
  // display.fillRect(0, 0, 128, 13, BLACK);
  update_battery("45%");
  display.drawBitmap(90, 0, battery_icon, 13, 13, 1);
  display.drawBitmap(0, 0, wifi_icon, 12, 12, 1);
}

/**
 * @brief Displays the no wifi icon on the OLED display
 *
 * This function displays the no wifi icon on the OLED display. The no wifi icon
 * is displayed at the top left corner of the display, and the battery icon is
 * displayed at the top right corner of the display.
 */
void OledDisplay::nowifi_icon()
{
  display.fillRect(0, 0, 14, 14, BLACK);

  display.drawBitmap(80, 0, battery_icon, 13, 13, 1);
  display.drawBitmap(0, 0, no_wifi_icon, 12, 12, 1);
  display.display();
}
