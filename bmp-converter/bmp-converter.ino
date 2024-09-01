/*********************************************************************
  This is an example for our Monochrome OLEDs based on SH110X drivers

  This example is for a 128x64 size display using I2C to communicate
  3 pins are required to interface (2 I2C and one reset)

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada  for Adafruit Industries.
  BSD license, check license.txt for more information
  All text above, and the splash screen must be included in any redistribution

  i2c SH1106 modified by Rupert Hirst  12/09/21
*********************************************************************/



#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define DEADBEEF_HEIGHT 64
#define DEADBEEF_WIDTH  128
static const unsigned char PROGMEM deadbeef[] = {
  B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,
  B11111111,B11111111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B11111111,B11111111,
  B11111111,B11100000,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B00000111,B11111111,
  B11111111,B00011111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111001,B11111111,
  B11111100,B11111111,B10000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000011,B11111110,B01111111,
  B11111011,B11111000,B01111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111100,B00111111,B10011111,
  B11110111,B11000111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11000111,B11101111,
  B11101111,B10111111,B11100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00001111,B11111001,B11110111,
  B11011110,B01111110,B00011111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11110000,B01111110,B01111011,
  B10111101,B11110001,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B10011111,B10111011,
  B10111011,B11101111,B11111000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00111111,B11100111,B11011101,
  B10110111,B10011111,B10000111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11000001,B11111001,B11011101,
  B10110111,B01111110,B01111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111110,B01111101,B11101101,
  B10110111,B01111001,B11111110,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B01111111,B10011110,B11101101,
  B10110110,B11110111,B11100001,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B10000111,B11101111,B01101101,
  B10110110,B11101111,B00011111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111001,B11110111,B01101101,
  B10110110,B11011110,B11111111,B10000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000001,B11111110,B01111011,B01101101,
  B10110110,B11011101,B11111000,B01111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111110,B00011111,B10111011,B01101101,
  B10110110,B11011011,B11100111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11100111,B11011011,B01101101,
  B10110110,B11011011,B10011111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111001,B11011011,B01101101,
  B10110110,B11011011,B01111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111110,B11011011,B01101101,
  B10110110,B11011011,B01111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111110,B11011011,B01101101,
  B10110110,B11011011,B01111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111110,B11011011,B01101101,
  B10110110,B11011011,B01111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111110,B11011011,B01101101,
  B10110110,B11011011,B01111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111110,B11011011,B01101101,
  B10110110,B11011011,B01111111,B11000011,B11111111,B11111111,B11111111,B11110000,B11100001,B11111111,B11111111,B11111111,B11111111,B00001110,B11011011,B01101101,
  B10110110,B11011011,B01111111,B11000001,B11111111,B11111111,B11111111,B11110000,B01000001,B11111111,B11111111,B11111111,B11111110,B00001110,B11011011,B01101101,
  B10110110,B11011011,B01111111,B11000001,B11111111,B11111111,B11111111,B11110000,B01100001,B11111111,B11111111,B11111111,B11111100,B00001110,B11011011,B01101101,
  B10110110,B11011011,B01111111,B11000011,B11110000,B11111111,B11111111,B11110000,B11100001,B11111111,B11000011,B11111000,B01111100,B00111110,B11011011,B01101101,
  B10110110,B11011011,B01111110,B11100011,B11000000,B01111100,B00011111,B10111000,B11100001,B10011111,B00000001,B11100000,B00111100,B00111110,B11011011,B01101101,
  B10110110,B11011011,B01111100,B00000011,B10000000,B00110000,B00001111,B00000000,B11110000,B00001110,B00000000,B11000000,B00010000,B00000110,B11011011,B01101101,
  B10110110,B11011011,B01111000,B00000011,B10000010,B00110000,B00000110,B00000000,B11100000,B00000110,B00001000,B11000001,B00010000,B00000110,B11011011,B01101101,
  B10110110,B11011011,B01111000,B11000011,B10000110,B00111001,B10000110,B00110000,B11100000,B00000110,B00011000,B11000011,B00011100,B00111110,B11011011,B01101101,
  B10110110,B11011011,B01111000,B11100011,B10000000,B00111100,B00000110,B00111000,B11100001,B10000110,B00000000,B11000000,B00011100,B00111110,B11011011,B01101101,
  B10110110,B11011011,B01110000,B11100001,B10000000,B01110000,B00000100,B00111000,B01100001,B11000110,B00000001,B11000000,B00111100,B00111110,B11011011,B01101101,
  B10110110,B11011011,B01111000,B11000001,B10000111,B11110001,B10000110,B00110000,B01100001,B11000110,B00011111,B11000011,B11111100,B00011110,B11011011,B01101101,
  B10110110,B11011011,B01111000,B00000001,B10000111,B00100011,B10000110,B00000000,B01100000,B10000110,B00011100,B11000011,B10011100,B00011110,B11011011,B01101101,
  B10110110,B11011011,B01111000,B00000001,B11000000,B00110000,B00000110,B00000000,B01100000,B00000111,B00000000,B11100000,B00011100,B00011110,B11011011,B01101101,
  B10110110,B11011011,B01111100,B00000001,B11000000,B00110000,B00000111,B00000000,B01100000,B00001111,B00000000,B11100000,B00011100,B00011110,B11011011,B01101101,
  B10110110,B11011011,B01111110,B11100011,B11110000,B01111000,B11000111,B10111000,B11110011,B10011111,B11000001,B11111000,B00111110,B00111110,B11011011,B01101101,
  B10110110,B11011011,B01111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111110,B11011011,B01101101,
  B10110110,B11011011,B01111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111110,B11011011,B01101101,
  B10110110,B11011011,B01111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111110,B11011011,B01101101,
  B10110110,B11011011,B01111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111101,B11011011,B01101101,
  B10110110,B11011011,B11011111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111011,B11011011,B01101101,
  B10110110,B11011011,B11100011,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11000111,B11011011,B01101101,
  B10110110,B11011100,B11111100,B00000111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11110000,B00111111,B10111011,B01101101,
  B10110110,B11011111,B01111111,B11111000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00001111,B11111110,B01111011,B01101101,
  B10110110,B11101111,B10001111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11110001,B11110111,B01101101,
  B10110110,B11110111,B11110000,B00001111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11110000,B00001111,B11001111,B01101101,
  B10110111,B01111001,B11111111,B11110000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00001111,B11111111,B00111110,B11101101,
  B10110111,B10111110,B00011111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111000,B11111101,B11101101,
  B10110111,B11001111,B11100000,B00011111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11110000,B00000111,B11110011,B11011101,
  B10111011,B11110011,B11111111,B11100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00001111,B11111111,B11001111,B11011101,
  B10111001,B11111100,B01111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111110,B00111111,B10111011,
  B11011110,B11111111,B10000000,B00111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111100,B00000001,B11111111,B01111011,
  B11011111,B00111111,B11111111,B11000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000011,B11111111,B11111100,B11110111,
  B11101111,B11001111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11110011,B11101111,
  B11110011,B11110001,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B00001111,B11011111,
  B11111100,B11111110,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B11111111,B00111111,
  B11111111,B00111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111100,B11111111,
  B11111111,B11000111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11000011,B11111111,
  B11111111,B11111100,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00111111,B11111111,
  B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111
};





void setup()   {

  Serial.begin(9600);

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.

  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
 //display.setContrast (0); // dim display
 
  display.display();
  delay(2000);
  // Clear the buffer.
  display.clearDisplay();

  // miniature bitmap display
  
  display.drawBitmap(0, 0,  deadbeef, DEADBEEF_WIDTH , DEADBEEF_HEIGHT, 1);
  display.display();
  delay(1);
}


void loop() {

}
