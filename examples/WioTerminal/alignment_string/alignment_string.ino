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

	render.setFontSize(30);
}

void loop()
{
	// put your main code here, to run repeatedly:

	/* Refresh grid line */
	refreshGridLine();

	/* You can select text alignment! use printf function */
	// Alignment Top
	render.setCursor(10, 30);
	render.setFontColor(TFT_CYAN);
	render.setAlignment(Align::TopLeft);
	render.printf("TopLeft");

	render.setCursor(160, 120);
	render.setFontColor(TFT_YELLOW);
	render.setAlignment(Align::TopCenter);
	render.printf("TopCenter");

	render.setCursor(310, 210);
	render.setFontColor(TFT_MAGENTA);
	render.setAlignment(Align::TopRight);
	render.printf("TopRight");

	delay(1500);

	/* Refresh grid line */
	refreshGridLine();

	/* You can select text alignment! use printf function */
	// Alignment Middle
	render.setCursor(10, 30);
	render.setFontColor(TFT_CYAN);
	render.setAlignment(Align::MiddleLeft);
	render.printf("MiddleLeft");

	render.setCursor(160, 120);
	render.setFontColor(TFT_YELLOW);
	render.setAlignment(Align::MiddleCenter);
	render.printf("MiddleCenter");

	render.setCursor(310, 210);
	render.setFontColor(TFT_MAGENTA);
	render.setAlignment(Align::MiddleRight);
	render.printf("MiddleRight");

	delay(1500);

	/* Refresh grid line */
	refreshGridLine();

	/* You can select text alignment! use printf function */
	// Alignment Bottom
	render.setCursor(10, 30);
	render.setFontColor(TFT_CYAN);
	render.setAlignment(Align::BottomLeft);
	render.printf("BottomLeft");

	render.setCursor(160, 120);
	render.setFontColor(TFT_YELLOW);
	render.setAlignment(Align::BottomCenter);
	render.printf("BottomCenter");

	render.setCursor(310, 210);
	render.setFontColor(TFT_MAGENTA);
	render.setAlignment(Align::BottomRight);
	render.printf("BottomRight");

	delay(1500);
}

void refreshGridLine()
{
	tft.fillScreen(TFT_BLACK);
	tft.drawFastVLine(10, 0, 240, TFT_DARKGREY);
	tft.drawFastVLine(160, 0, 240, TFT_DARKGREY);
	tft.drawFastVLine(310, 0, 240, TFT_DARKGREY);
	tft.drawFastHLine(0, 30, 320, TFT_DARKGREY);
	tft.drawFastHLine(0, 120, 320, TFT_DARKGREY);
	tft.drawFastHLine(0, 210, 320, TFT_DARKGREY);
}