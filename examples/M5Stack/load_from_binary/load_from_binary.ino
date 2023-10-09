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

	// render.setBackgroundFillMethod(BgFillMethod::None);	// default
	// render.setBackgroundFillMethod(BgFillMethod::Minimum);
	// render.setBackgroundFillMethod(BgFillMethod::Block);

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
