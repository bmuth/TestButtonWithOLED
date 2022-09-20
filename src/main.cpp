#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

bool d (true);

const int16_t SCREEN_WIDTH = 128; // OLED display width, in pixels
const int16_t SCREEN_HEIGHT = 64; // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
const int16_t OLED_RESET = -1; // Reset pin # (or -1 if sharing Arduino reset pin)
const int16_t SCREEN_ADDRESS =  0x3C; //< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

class Display : public Adafruit_SSD1306
{
public:
	Display ()
	: Adafruit_SSD1306 (SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET)
	{
	}
	void clearLine (int16_t line)
	{
		Adafruit_GFX::fillRect (0, line, SCREEN_WIDTH, 8, SSD1306_BLACK);
	}
	void print (int16_t col, int16_t row, const char *s)
	{
		setCursor (col, row);
		Adafruit_GFX::print (s);
	}
	void print (const char *s)
	{
		Adafruit_GFX::print (s);
	}
	void print (String s)
	{
		Adafruit_GFX::print (s);
	}
	void print (int i)
	{
		Adafruit_GFX::print (i);
	}

};

Display display;

#define LED 2  // digital pin connected to LED, for testing of switch code only
bool led_status = LOW; // start with LED off, for testing of switch code only
/*
  DigitalReadSerial

  Reads a digital input on pin 2, prints the result to the Serial Monitor

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/DigitalReadSerial
*/

int pushButton = 17;

// the setup routine runs once when you press reset:
void setup () 
{
	pinMode (LED, OUTPUT);


	// initialize serial communication at 9600 bits per second:
	Serial.begin (9600);
	while (!Serial)
	{}
	// make the pushbutton's pin an input:
	pinMode (pushButton, INPUT);

	if (d)
	{
		// Wire.begin ();
		// Wire.setClock (100000);



	// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
		if(!display.begin (SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
		{
			Serial.println(F("SSD1306 allocation failed"));
			for(;;); // Don't proceed, loop forever
		}

		display.display ();
		delay (2000); // Pause for 2 seconds

		// Clear the buffer
		display.clearDisplay ();

		display.setTextSize (1);      // Normal 1:1 pixel scale
		display.setTextColor (SSD1306_WHITE); // Draw white text
		display.setCursor (0, 0);     // Start at top-left corner
		display.cp437 (true);         // Use full 256 char 'Code Page 437' font

		// Not all the characters will fit on the display. This is normal.
		// Library will draw what it can and the rest will be clipped.
		for(int16_t i=0; i<256; i++) 
		{
			if(i == '\n') display.write(' ');
			else          display.write(i);
		}

		display.display ();
		delay (2000);
		display.clearDisplay ();
	}
}

// the loop routine runs over and over again forever:
void loop () 
{
	static int oldstate = 0;
	// read the input pin:
	int buttonState = digitalRead (pushButton);
	// print out the state of the button:
	if (buttonState == LOW)
	{
		Serial.print ("LOW\n");
	}
	else
	{
		Serial.print ("HIGH\n");
	}
	if (oldstate != buttonState)
	{
		if (buttonState == 1)
		{
			digitalWrite (LED, HIGH); 
			Serial.println ("State changed to HIGH"); 
			if (d)
			{
				display.clearLine (20);
				display.print (0, 20, "HIGH");
				display.display ();
			}
		}
		else
		{
			digitalWrite (LED, LOW);
			Serial.println ("State changed to LOW");
			if (d)
			{
				display.clearLine (20);
				display.print (0, 20, "LOW  ");
				display.display ();
			}
		}
		oldstate = buttonState;
	}
	delay (500);
}
