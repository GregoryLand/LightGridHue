// Hack Pittsburgh Ligt Grid Hue test program

#include <stdint.h>
#include <SoftwareSerial.h>
#include <G35String.h>
#include <G35StringGroup.h>
#include "led_utils.h"

SoftwareSerial LCD(2,3);


const int NUMBER_OF_STRINGS = 2;
const int NUMBER_OF_LEDS = NUMBER_OF_STRINGS * LEDS_PER_STRING;

const int MAX_HUE = 255;
const int HALF_HUE = (MAX_HUE+1)/2;
const int MAX_COLOR = 15;
const int HALF_COLOR = (MAX_COLOR+1)/2;

uint16_t hueColors[MAX_HUE + 1];

void initializeColors() {
  int red, green, blue;
  
  for (int hue = 0; hue <= MAX_HUE; hue++) {
    if (hue < HALF_HUE) {
      green = (hue+1)*MAX_COLOR/HALF_HUE;
      red = MAX_COLOR - green;
       blue = 0;
    } else {
      red = 0;
      green = (MAX_HUE+1-hue)*MAX_COLOR/HALF_HUE;
      blue = MAX_COLOR-green;
    }

    int color = blue << 8 | green << 4 | red;
    hueColors[hue] = color;
  }
}

void setup() 
{
  Serial.begin(9600);

  LCD.begin(9600);
  LCD.write(12); // Form Feed, clear screen
  LCD.write(17); // Turn on backlight (Parallax LCD)
  LCD.print("Light Grid Hue");
  LCD.write(148); // move to line 1 pos 0

  initializeLedBoard();

  initializeColors();

  clearscreen();
  delay(200);

  int brightness = 128;
  for (int led = 0; led < NUMBER_OF_LEDS; led++) {
      int hue = (led * MAX_HUE)/(NUMBER_OF_LEDS-1);
      setLed(led, brightness, hueColors[hue]);
  }
}

void loop() 
{
}








