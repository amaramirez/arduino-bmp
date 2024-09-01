# arduino-bmp

Little tool for converting monochrome bitmaps to Arduino-friendly indexed arrays.
  

Just run `python3 bmp2oled.py` inside of the `bmp2oled` folder, it has a handy interface for selecting files for converting.

  

![Screenshot of interface](https://i.imgur.com/hoNYTB9.png)

  

The files are relative to the script location and will only show BMP files in the same directory as the script.



It will output a text file with C++ code you can copy into your sketch (sample sketch included)

## Example

```

#define SMILE_HEIGHT 16
#define SMILE_WIDTH  16
static const unsigned char PROGMEM smile[] = {
	B00000111,B11100000,
	B00001000,B00010000,
	B00110000,B00001100,
	B00100000,B00000100,
	B01000000,B00000010,
	B10000100,B00100010,
	B10000000,B00000001,
	B10000000,B00000001,
	B10000000,B00000001,
	B10001000,B00100001,
	B10001000,B00100010,
	B01001100,B01100010,
	B00100011,B10000100,
	B00110000,B00001100,
	B00001100,B00110000,
	B00000011,B11000000
};

//Copy this to after initialization
display.clearDisplay();
display.drawBitmap(0, 0,  smile, SMILE_WIDTH , SMILE_HEIGHT, 1);
display.display();


```



The only setting you may need to change depends on the environment you are running. This command helps with keeping the interface clean and navigatable. Use the top line for MacOS/Linux and the bottom for Windows

  

![Screenshot of clear command](https://i.imgur.com/T1gjlNn.png)
