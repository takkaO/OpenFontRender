#if defined(ARDUINO_M5Stack_Core_ESP32) || defined(ARDUINO_M5STACK_FIRE)
#include "M5Stack.h"
#elif defined(ARDUINO_M5STACK_Core2) // M5Stack Core2
#include "M5Core2.h"
#endif
#include "binaryttf.h"
#include "OpenFontRender.h" // Include after M5Stack.h / M5Core2.h

OpenFontRender render;

void setup()
{
	// put your setup code here, to run once:
	M5.begin();
	M5.Lcd.fillScreen(BLACK);

	render.showFreeTypeVersion(); // print FreeType version
	render.showCredit();		  // print FTL credit

	if (render.loadFont(binaryttf, sizeof(binaryttf)))
	{
		Serial.println("Render initialize error");
		return;
	}

	render.setDrawer(M5.Lcd); // Set drawer object
	/* You can also be written as follows. */
	// render.setDrawPixel(M5.Lcd.drawPixel);
	// render.setStartWrite(M5.Lcd.startWrite); // optional
	// render.setEndWrite(M5.Lcd.endWrite);	 // optional

	render.setFontSize(25);
}

void loop()
{
	// put your main code here, to run repeatedly:
	
	/* You can select text alignment! use printf function */
	// Alignment Left
	render.setCursor(0, 0);
	render.setFontColor(WHITE);
	render.printf("Hello\nWorld");

	// Alignment Center
	render.setCursor(160, 95);
	render.setFontColor(GREEN);
	render.cprintf(u8"完全なUnicode\nサポート");

	// Alignment Right
	render.setCursor(320, 190);
	render.setFontColor(ORANGE);
	render.rprintf(u8"こんにちは\n世界");


	delay(1000);
	//M5.Lcd.fillScreen(BLACK);

	/* You can select text alignment! use drawString function */
	// Alignment Left
	render.drawString("Hello\nWorld", 0, 0, TFT_MAGENTA, TFT_BLACK);

	// Alignment Center
	render.cdrawString(u8"完全なUnicode\nサポート", 160, 95, TFT_YELLOW, TFT_BLACK);

	// Alignment Right
	render.rdrawString(u8"こんにちは\n世界", 320, 190, TFT_CYAN, TFT_BLACK);


	delay(1000);
	//M5.Lcd.fillScreen(BLACK);
}
