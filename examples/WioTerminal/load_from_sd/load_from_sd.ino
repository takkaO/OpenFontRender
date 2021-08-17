#include "TFT_eSPI.h"
#include "OpenFontRender.h"

TFT_eSPI tft;
OpenFontRender render;

void setup()
{
	// put your setup code here, to run once:
	Serial.begin(115200);
	Serial.flush();
    delay(50);
	
	if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI))
	{
		Serial.println("SD initialization failed!");
		return;
	}
	tft.begin();
	tft.setRotation(3);
	tft.fillScreen(TFT_BLACK);
	digitalWrite(LCD_BACKLIGHT, HIGH); // turn on the backlight

	render.showFreeTypeVersion(); // print FreeType version
	render.showCredit();		  // print FTL credit

	if (render.loadFont("/JKG-M_3_Tiny.ttf"))
	{
		Serial.println("Render initialize error");
		return;
	}

	render.setDrawer(tft); // Set drawer object
	/* You can also be written as follows. */
	// render.setDrawPixel(tft.drawPixel);
	// render.setStartWrite(tft.startWrite); // optional
	// render.setEndWrite(tft.endWrite);	 // optional

	unsigned long t_start = millis();

	render.setFontColor(TFT_WHITE);
	render.printf("Hello World\n");
	render.seekCursor(0, 10);

	render.setFontSize(30);
	render.setFontColor(TFT_GREEN);
	render.printf("完全なUnicodeサポート\n");
	render.seekCursor(0, 10);

	render.setFontSize(40);
	render.setFontColor(TFT_ORANGE);
	render.printf("こんにちは世界\n");

	unsigned long t_end = millis();
	Serial.printf("Time: %ld ms\n", t_end - t_start);
}

void loop()
{
	// put your main code here, to run repeatedly:
}
