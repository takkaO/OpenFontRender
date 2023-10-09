#include <Arduino.h>
#include <M5Unified.h>

#include "binaryttf.h"
#include "OpenFontRender.h" // Include after M5Unified.h

OpenFontRender render;

void setup()
{
	// put your setup code here, to run once:
	auto cfg = M5.config();
	M5.begin(cfg);
	M5.Display.fillScreen(TFT_BLACK);

	render.setSerial(Serial);	  // Need to print render library message
	render.showFreeTypeVersion(); // print FreeType version
	render.showCredit();		  // print FTL credit

	if (render.loadFont(binaryttf, sizeof(binaryttf)))
	{
		Serial.println("Render initialize error");
		return;
	}

	render.setDrawer(M5.Display); // Set drawer object
	/* You can also be written as follows. */
	// render.setDrawPixel(M5.Display.drawPixel);
	// render.setDrawFastHLine(M5.Display.drawFastHLine); // optional
	// render.setStartWrite(M5.Display.startWrite);       // optional
	// render.setEndWrite(M5.Display.endWrite);           // optional

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
	M5.Display.fillScreen(TFT_BLACK);
	M5.Display.drawFastVLine(10, 0, 240, TFT_DARKGREY);
	M5.Display.drawFastVLine(160, 0, 240, TFT_DARKGREY);
	M5.Display.drawFastVLine(310, 0, 240, TFT_DARKGREY);
	M5.Display.drawFastHLine(0, 30, 320, TFT_DARKGREY);
	M5.Display.drawFastHLine(0, 120, 320, TFT_DARKGREY);
	M5.Display.drawFastHLine(0, 210, 320, TFT_DARKGREY);
}