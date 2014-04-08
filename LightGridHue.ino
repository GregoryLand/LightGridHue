// Hack Pittsburgh Ligt Grid Hue test program
// Greg: I am assuming that uint8_t is big enough to hold all of the lights
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
uint8_t  randomArrayOfNonrepeatingNumbers[NUMBER_OF_LEDS];

void initializeColors() {
  int red, green, blue;
  
  for (int hue = 0; hue <= MAX_HUE; hue++) 
  {
    if (hue < HALF_HUE) 
    {
      green = (hue+1)*MAX_COLOR/HALF_HUE;
      red = MAX_COLOR - green;
       blue = 0;
    } 
    else
    {
      red = 0;
      green = (MAX_HUE+1-hue)*MAX_COLOR/HALF_HUE;
      blue = MAX_COLOR-green;
    }

    int color = blue << 8 | green << 4 | red;
    hueColors[hue] = color;
  }
}

// This function returns a color in the spectrum based on a number
// Between 0 and MAX_NUMBER_OF_LEDS
uint16_t getHue(uint8_t step0toMAX_NUMBER_OF_LEDS)
{
  return hueColors[ (step0toMAX_NUMBER_OF_LEDS * MAX_HUE)/(NUMBER_OF_LEDS-1) ];
}

// This function generates a temp array with numbers 0 to (NUMBER_OF_LEDS - 1) 
// We use this array as a grab bag to pull numbers out of randomly so we dont repeat
// numbers in the array to display.  This makes sure there is only one of each color 
// on the display and that the distribution is nice and random
void randomizeRandomArrayOfNonrepeatingNumbers()
{
  // Set each number in temp to its position in the array
  uint8_t temp[NUMBER_OF_LEDS];
  for(int x = 0; x < NUMBER_OF_LEDS; x++)
  {
    temp[x] = x;
  }
  
  // We will grab a random item from the array and them copy the last item in the array into the old position
  // And remove bump down our item count to keep track of the end of the array
  int UpperBoundOfTempArray = NUMBER_OF_LEDS;
  for(int x = 0; x < NUMBER_OF_LEDS; x++)
  {
    // Get a random number between 0 and upper bound exclusive
    int randomLocation = random(0, UpperBoundOfTempArray);
    
    // Set randArrayOfNonRepeaters to contain the number that was in the random temp array
    randomArrayOfNonrepeatingNumbers[x] = temp[randomLocation];
    
    // move last item in array to position of item removed from array
    temp[randomLocation] = temp[UpperBoundOfTempArray-1];
    
    // Drop our upper bound
    UpperBoundOfTempArray--;
  }
}

// Loops though the array and draws each pixel to the screen
void drawEntireScreen()
{
  int brightness = 128;
  for (int led = 0; led < NUMBER_OF_LEDS; led++) {
      setLed(led, brightness, getHue(randomArrayOfNonrepeatingNumbers[led]) );
  }
}

// Meh looks pretty this is art not science....
boolean doSingleBubbleSortPass()
{
  boolean didWeSwap = false;
  // Walk though the array and check each pair
  for( uint8_t x = 0; x < NUMBER_OF_LEDS - 1; x++ )
  { 
    // Test if value on right is bigger if not swap
    if( randomArrayOfNonrepeatingNumbers[x] > randomArrayOfNonrepeatingNumbers[x+1] )
    {
      didWeSwap = true;
      // Swap the values
      uint8_t temp = randomArrayOfNonrepeatingNumbers[x+1];
      randomArrayOfNonrepeatingNumbers[x+1] = randomArrayOfNonrepeatingNumbers[x];
      randomArrayOfNonrepeatingNumbers[x] = temp;
    }
  }
  return didWeSwap;
}

void setup() 
{
  // Start Random Number Gen
  randomSeed(1337);

  Serial.begin(9600);

  LCD.begin(9600);
  LCD.write(12); // Form Feed, clear screen
  LCD.write(17); // Turn on backlight (Parallax LCD)
  LCD.print("Light Grid Hue");
  LCD.write(148); // move to line 1 pos 0

  initializeLedBoard();
  initializeColors();
}

void loop() 
{
  // Setup the random array  
  randomizeRandomArrayOfNonrepeatingNumbers();
  
  // Clear screen to start over
  clearscreen();
  delay(200);

  // Draw the random array 
  drawEntireScreen();
  
  // Start Sort Algorthim
  for( uint8_t x = 0; x < NUMBER_OF_LEDS; x++ )
  {
    delay(1000);
    boolean didWeSortOnLastPass = doSingleBubbleSortPass();
    drawEntireScreen();
    if( didWeSortOnLastPass == false ) break;
  }
 
  // Pause Before Reseting
  delay(10000);
}
