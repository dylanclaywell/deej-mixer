/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/


#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10
#define TFT_MOSI 24
#define TFT_MISO 23
#define TFT_CLK 25
#define TFT_RST -1

#define VOLUME_PIN 19
#define VOLUME_MAX_VALUE 1024

#define RECT_WIDTH 50
#define RECT_HEIGHT 200

// My screen uses the breakout board so we can initialize it with all of the SPI pins
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

float volumeKnobValue = 0.0;

void setup() {
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
}

void loop(void) {
  volumeKnobValue = analogRead(VOLUME_PIN);

  tft.setCursor(10, 220);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);  
  tft.setTextSize(1);
  tft.drawRect(10, 220, 50, 24, ILI9341_BLACK);

  // Every character in a string that is printed moves the cursor, so we first print the value, then print some spaces to erase any text longer than the value
  tft.print(volumeKnobValue);
  tft.print("    ");

  tft.drawRect(10, 10, RECT_WIDTH, RECT_HEIGHT, ILI9341_WHITE);

  float blueHeight = ((volumeKnobValue / VOLUME_MAX_VALUE) * (RECT_HEIGHT - 7));
  float blueY = ((RECT_HEIGHT - 7) - blueHeight);

  float blackHeight = ((RECT_HEIGHT) - blueHeight) - 7;
  float blackY = 14;

  tft.fillRect(14, blueY + 14, RECT_WIDTH - 7, blueHeight, ILI9341_CYAN);
  tft.fillRect(14, blackY, RECT_WIDTH - 7, blackHeight, ILI9341_BLACK);

  tft.setCursor(10, 248);
  tft.print("Height: ");
  tft.print(blueHeight);
  tft.print("    ");
  tft.print(blackHeight);
  tft.print("    ");

  tft.setCursor(10, 272);
  tft.print("Y: ");
  tft.print(blueY);
  tft.print("    ");
  tft.print(blackY);
  tft.print("    ");

  tft.setCursor(10, 296);
  tft.print("Value: ");
  tft.print(volumeKnobValue);
  tft.print("  Max: ");
  tft.print(VOLUME_MAX_VALUE);
  tft.print("    ");
}