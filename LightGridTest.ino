// Hack Pittsburgh Charactor Test program

#include <SoftwareSerial.h>

SoftwareSerial LCD(2,3);

#include <G35String.h>
#include <G35StringGroup.h>

#include "led_utils.h"

int brightness;
int led = -1;
unsigned int color = 0x0444;
char inchar;
int scrollFlag = 0;

void setup() 
{
  Serial.begin(9600);

  LCD.begin(9600);
  LCD.write(12); // Form Feed, clear screen
  LCD.write(17); // Turn on backlight (Parallax LCD)
  LCD.print("Light Grid Test");
  LCD.write(148); // move to line 1 pos 0

  initializeLedBoard();

  clearscreen();
  delay(200);
}

void loop() 
{
  if ( scrollFlag )
  {
    scrollText("ABCDEFGHIJKLMNOPQRSTUVWXZ 0123456789", color, FONT_5_X_7) ;
    scrollText("ABCDEFGHIJKLMNOPQRSTUVWXZ 0123456789", color, FONT_3_X_5) ;
    scrollFlag = 0;
  }

  if (Serial.available ())
  {
    clearscreen();
    inchar = Serial.read();
    Serial.print("CHAR = ");
    Serial.println(inchar);
    if ( inchar == 0x053 || inchar == 0x073 ) scrollFlag = 1 ;
    if ( inchar == 0x052 || inchar == 0x072  ) { 
      color = 0x008; //  Red
      brightness = 170; 
    }
    if ( inchar == 0x047 || inchar == 0x067  ) { 
      color = 0x080; //  Green
      brightness = 170; 
    }
    if ( inchar == 0x042 || inchar == 0x062  ) { 
      color = 0x800; //  Blue
      brightness = 170; 
    }
    if ( inchar >= 0x20 && inchar <= 0x7F ) outChar( inchar, color, -1, FONT_5_X_7);
    if ( inchar >= 0x20 && inchar <= 0x7F ) outChar( inchar, color, 6, FONT_3_X_5);
    LCD.write(12); // Form Feed, clear screen
    LCD.write(148); // move to line 1 pos 0
    LCD.print("CHAR = ");
    LCD.print(inchar);
    led = -1;
    delay(1000);
    clearscreen();
  }
  else
  {
    LCD.write(148); // move to line 1 pos 0
    if ( led < 71 )
    {
      if ( led > -1 ) setLed(led, 125, COLOR_BLACK);
      led++;
      setLed(led, 125, color);
      LCD.print("LED Number = ");
      LCD.print(led);
      Serial.print("LED Number = ");
      Serial.println(led);
      delay(250);
    }
    else
    {
      if ( brightness < 0x0cc ) brightness = brightness + 5;
      else
      {
        if ( color < 0x0fff )
        {
          LCD.write(12); // Form Feed, clear screen
          LCD.print("color = ");
          LCD.print(color, HEX);
          color++;
          brightness = 0;
          LCD.write(12); // Form Feed, clear screen
          LCD.print("color = ");
          LCD.print(color, HEX);
        }
        else
        {
          color = 0;
          brightness = 5;
          led = -1;
        }
        delay(5000);
      }
      setLed(23, brightness, color);
      setLed(24, brightness, color);
      setLed(25, brightness, color);
      setLed(30, brightness, color);
      setLed(31, brightness, color);
      setLed(32, brightness, color);
      setLed(37, brightness, color);
      setLed(38, brightness, color);
      setLed(39, brightness, color);
      setLed(44, brightness, color);
      setLed(45, brightness, color);
      setLed(46, brightness, color);
      Serial.print("color = ");
      Serial.print(color, HEX);
      Serial.print(" Red = ");
      Serial.print((color & 0x000f), HEX);
      Serial.print(" Green = ");
      Serial.print(((color >> 4) & 0x000f), HEX);
      Serial.print(" Blue = ");
      Serial.print(((color >> 8) & 0x000f), HEX);
      Serial.print(" bright = ");
      Serial.println(brightness);
      LCD.write(148); // move to line 1 pos 0
      LCD.print("bright = ");
      LCD.print(brightness);
      delay(100);
    }
  }
}








