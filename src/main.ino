/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

struct Bitmap {
  const uint8_t* data;
  uint8_t width;
  uint8_t height;
};

// Bitmap data stored in PROGMEM to save RAM
const uint8_t PROGMEM shieldData[] = {
  0x1f, 0xc0, 0x7f, 0xf0, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xc0, 0x18, 0xc0, 0x18, 0xc0, 0x18, 
  0xf8, 0xf8, 0x78, 0xf0, 0x38, 0xe0, 0x1f, 0xc0, 0x0f, 0x80, 0x07, 0x00
};

const uint8_t PROGMEM bigCrossData[] = {
  0x00, 0x00, 0x00, 0x00, 
  0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x3f, 0xe0, 0x3f, 0xe0, 0x3f, 0xe0, 0x07, 0x00, 0x07, 0x00, 
  0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t PROGMEM smallFlagData[] = {
  0xf8, 0xd8, 0x88, 0xd8, 0xf8
};

const uint8_t PROGMEM smallCrossData[] = {
  0x00, 0x20, 0x70, 0x20, 0x00
};

// Bitmap objects
Bitmap shield = {shieldData, 13, 14};
Bitmap bigCross = {bigCrossData, 13, 14};
Bitmap smallFlag = {smallFlagData, 5, 5};
Bitmap smallCross = {smallCrossData, 5, 5};

// Menu variables
int selected = 0;
const char* menuItems[] = {"A Alarm", "C Alarm"};
const int menuItemCount = 2;
const int menuItemsPerCol = 2;
Bitmap selectedBitmap = shield;
Bitmap unselectedBitmap = bigCross;
void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();
  display.setFont(&FreeSans9pt7b);
}

void loop() {
  render();
  delay(100); // Adjust refresh rate as needed
  
  // Add your input handling here (buttons, etc.)
  // Example:
  // if (buttonPressed) {
  //   selected = (selected + 1) % menuItemCount;
  // }
}

void render() {
  display.clearDisplay();

  for (int i = 0; i < menuItemCount; i++) {
    int x = 1 + (i / menuItemsPerCol) * (display.width() / ((menuItemCount + menuItemsPerCol - 1) / menuItemsPerCol));
    int y = (i % menuItemsPerCol) * (display.height() / menuItemsPerCol) + 1;
    
    Bitmap bitmap = (i == selected) ? selectedBitmap : unselectedBitmap;
    
    display.drawBitmap(x, y, bitmap.data, bitmap.width, bitmap.height, WHITE);
    
    display.setCursor(x + bitmap.width + 1, y+12);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.print(menuItems[i]);
  }
  
  display.display();
}