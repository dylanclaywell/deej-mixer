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
#include "TFT_eSPI.h"
#include "TimerEvent.h"

// For the Feather M4 Express, these are the pins used
//
// Make sure to update the User_Setup.h file in the TFT_eSPI library to make changes
//
// TFT_DC 9
// TFT_CS 10
// TFT_MOSI 24
// TFT_MISO 23
// TFT_CLK 25
// TFT_RST -1

#define ANALOG_PIN_2 16
#define ANALOG_PIN_3 17
#define ANALOG_PIN_4 18
#define ANALOG_PIN_5 19
#define VOLUME_MAX_VALUE 1023

#define RADIUS 36
#define PADDING_LEFT 36
#define PADDING_RIGHT 40
#define PADDING_TOP 24
#define PADDING_BOTTOM 24

#define RECT_WIDTH 50
#define RECT_HEIGHT 200

TFT_eSPI tft = TFT_eSPI();

enum Direction
{
  UP,
  DOWN
};

enum VolumeKnobIds
{
  NONE,
  KNOB1,
  KNOB2,
  KNOB3,
  KNOB4
};

class VolumeKnob
{
public:
  int x;
  int y;
  int radius;
  int color;
  VolumeKnobIds id;
  int pin;
  uint16_t last_angle = 30;
  float volumeKnobValue;
  char *displayName;

  int growCounter = 0;
  TFT_eSprite img = TFT_eSprite(&tft);

  VolumeKnob(int xx, int yy, int r, int c, VolumeKnobIds i, int p, char *dn)
  {
    x = xx;
    y = yy;
    radius = r;
    color = c;
    id = i;
    pin = p;
    displayName = dn;
  }

  int getMeterAngle()
  {
    volumeKnobValue = analogRead(pin);
    float percentageFilled = (volumeKnobValue / (VOLUME_MAX_VALUE));
    // Range here is 0-100 so value is scaled to an angle 30-330
    return map(percentageFilled * 100, 0, 100, 30, 330);
  }

  void draw()
  {
    int angle = getMeterAngle();

    float diameter = 2 * radius;

    img.createSprite((diameter) + 6, (diameter) + 6);

    img.fillRect(0, 0, (diameter) + 6, (diameter) + 6, TFT_BLACK);
    img.fillCircle(radius + 3, radius + 3, radius, TFT_BLACK);
    img.drawSmoothCircle(radius + 3, radius + 3, radius + 2, TFT_SILVER, TFT_DARKGREY);

    int r = radius - 3;

    // Allocate a value to the arc thickness dependant of r
    uint8_t thickness = r / 5;
    if (r < 25)
      thickness = r / 3;

    img.drawArc(radius + 3, radius + 3, r, r - thickness, 30, angle, color, TFT_BLACK);

    last_angle = angle; // Store meter arc position for next redraw

    img.pushSprite(x, y, TFT_TRANSPARENT);

    tft.drawRect(x + diameter + 6.0, y, 2, diameter + 6.0, TFT_BLACK);
    tft.drawRect(x, y + diameter + 6.0, diameter + 6.0, 2, TFT_BLACK);

    img.deleteSprite();

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(x, y + diameter + 10);
    tft.print(displayName);
  }

  void update()
  {
    int angle = getMeterAngle();

    draw();
  };

  void init()
  {
    volumeKnobValue = analogRead(pin);
    last_angle = volumeKnobValue;
    uint16_t tmp = radius - 3;
  }
};

float diameter = 2 * RADIUS;

VolumeKnob volumeKnob1 = VolumeKnob(PADDING_LEFT, PADDING_TOP, RADIUS, TFT_DARKCYAN, KNOB1, ANALOG_PIN_5, "Main");
VolumeKnob volumeKnob2 = VolumeKnob(TFT_HEIGHT - diameter - PADDING_RIGHT, PADDING_TOP, RADIUS, TFT_DARKGREEN, KNOB2, ANALOG_PIN_4, "Firefox");
VolumeKnob volumeKnob3 = VolumeKnob(PADDING_LEFT, TFT_WIDTH - diameter - PADDING_BOTTOM, RADIUS, TFT_MAROON, KNOB3, ANALOG_PIN_3, "Discord");
VolumeKnob volumeKnob4 = VolumeKnob(TFT_HEIGHT - diameter - PADDING_RIGHT, TFT_WIDTH - diameter - PADDING_BOTTOM, RADIUS, TFT_PURPLE, KNOB4, ANALOG_PIN_2, "Other");

void sendValues()
{
  String builtString = String("");
  int values[4] = {volumeKnob1.volumeKnobValue, volumeKnob2.volumeKnobValue, volumeKnob3.volumeKnobValue, volumeKnob4.volumeKnobValue};

  for (int i = 0; i < 4; i++)
  {
    builtString += String((int)values[i]);

    if (i < 4 - 1)
    {
      builtString += String("|");
    }
  }

  Serial.println(builtString);
}

void setup()
{
  Serial.begin(9600);

  tft.begin();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);

  volumeKnob1.init();
  volumeKnob2.init();
  volumeKnob3.init();
  volumeKnob4.init();
}

void loop(void)
{
  volumeKnob1.update();
  volumeKnob2.update();
  volumeKnob3.update();
  volumeKnob4.update();

  sendValues();
}
