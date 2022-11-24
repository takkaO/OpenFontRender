#include "TFT_eSPI.h"
#include "binaryttf.h"
#include "OpenFontRender.h"

TFT_eSPI tft;
OpenFontRender render;

void setup()
{
	// put your setup code here, to run once:
	Serial.begin(115200);
	Serial.flush();
	delay(50);

	tft.begin();
	tft.setRotation(3);
	tft.fillScreen(TFT_BLACK);
	digitalWrite(LCD_BACKLIGHT, HIGH); // turn on the backlight

	render.setSerial(Serial);	  // Need to print render library message
	render.showFreeTypeVersion(); // print FreeType version
	render.showCredit();		  // print FTL credit

	if (render.loadFont(binaryttf, sizeof(binaryttf)))
	{
		Serial.println("Render initialize error");
		return;
	}

	render.setDrawer(tft); // Set drawer object
	/* You can also be written as follows. */
	// render.setDrawPixel(tft.drawPixel);
	// render.setDrawFastHLine(tft.drawFastHLine); // optional
	// render.setStartWrite(tft.startWrite);       // optional
	// render.setEndWrite(tft.endWrite);           // optional

	render.setFontSize(25);
}

void loop()
{
	// put your main code here, to run repeatedly:

	/* You can select text alignment! use printf function */
	// Alignment Left
	render.setCursor(0, 0);
	render.setFontColor(TFT_WHITE);
	render.printf("Hello\nWorld");

	// Alignment Center
	render.setCursor(160, 95);
	render.setFontColor(TFT_GREEN);
	render.cprintf("完全なUnicode\nサポート");

	// Alignment Right
	render.setCursor(320, 190);
	render.setFontColor(TFT_ORANGE);
	render.rprintf("こんにちは\n世界");

	delay(1000);
	// M5.Lcd.fillScreen(BLACK);

	/* You can select text alignment! use drawString function */
	// Alignment Left
	render.drawString("Hello\nWorld", 0, 0, TFT_MAGENTA, TFT_BLACK);

	// Alignment Center
	render.cdrawString("完全なUnicode\nサポート", 160, 95, TFT_YELLOW, TFT_BLACK);

	// Alignment Right
	render.rdrawString("こんにちは\n世界", 320, 190, TFT_CYAN, TFT_BLACK);

	delay(1000);
}
