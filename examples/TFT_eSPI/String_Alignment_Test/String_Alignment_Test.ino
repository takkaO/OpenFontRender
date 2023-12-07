#include "OpenFontRender.h"
OpenFontRender ofr;

#include "NotoSans_Bold.h"
#define TTF_FONT NotoSans_Bold // The font is referenced with the array name

#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

#define TXT_BG_COLOR TFT_RED
//#define TXT_BG_COLOR TFT_BLACK

void setup(void) {
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  ofr.setDrawer(tft);
  ofr.loadFont(TTF_FONT, sizeof(TTF_FONT));
  ofr.setBackgroundFillMethod(BgFillMethod::Block);

  refreshGridLine();
}

void loop() { 
  ofr.setFontSize(36);
  ofr.setFontColor(TFT_WHITE, TFT_BLACK);

	/* You can select text alignment! use printf function */
	// Alignment Top
	ofr.setCursor(10, 30);
	ofr.setFontColor(TFT_CYAN, TXT_BG_COLOR);
	ofr.setAlignment(Align::TopLeft);
	ofr.printf("TopLeft");

	ofr.setCursor(160, 120);
	ofr.setFontColor(TFT_YELLOW, TXT_BG_COLOR);
	ofr.setAlignment(Align::TopCenter);
	ofr.printf("TopCenter");

	ofr.setCursor(310, 210);
	ofr.setFontColor(TFT_MAGENTA, TXT_BG_COLOR);
	ofr.setAlignment(Align::TopRight);
	ofr.printf("TopRight");

	delay(3500);

	/* Refresh grid line */
	refreshGridLine();

	/* You can select text alignment! use printf function */
	// Alignment Middle
	ofr.setCursor(10, 30);
	ofr.setFontColor(TFT_CYAN, TXT_BG_COLOR);
	ofr.setAlignment(Align::MiddleLeft);
	ofr.printf("MiddleLeft");

	ofr.setCursor(160, 120);
	ofr.setFontColor(TFT_YELLOW, TXT_BG_COLOR);
	ofr.setAlignment(Align::MiddleCenter);
	ofr.printf("MiddleCenter");

	ofr.setCursor(310, 210);
	ofr.setFontColor(TFT_MAGENTA, TXT_BG_COLOR);
	ofr.setAlignment(Align::MiddleRight);
	ofr.printf("MiddleRight");

	delay(3500);

	/* Refresh grid line */
	refreshGridLine();

	/* You can select text alignment! use printf function */
	// Alignment Bottom
	ofr.setCursor(10, 30);
	ofr.setFontColor(TFT_CYAN, TXT_BG_COLOR);
	ofr.setAlignment(Align::BottomLeft);
	ofr.printf("BottomLeft");

	ofr.setCursor(160, 120);
	ofr.setFontColor(TFT_YELLOW, TXT_BG_COLOR);
	ofr.setAlignment(Align::BottomCenter);
	ofr.printf("BottomCenter");

	ofr.setCursor(310, 210);
	ofr.setFontColor(TFT_MAGENTA, TXT_BG_COLOR);
	ofr.setAlignment(Align::BottomRight);
	ofr.printf("BottomRight");

	delay(3500);

  refreshGridLine();
}

void refreshGridLine() {
	tft.fillScreen(TFT_BLACK);
	tft.drawFastVLine(10,   0, 240, TFT_DARKGREY);
	tft.drawFastVLine(160,  0, 240, TFT_DARKGREY);
	tft.drawFastVLine(310,  0, 240, TFT_DARKGREY);
	tft.drawFastHLine(0,   30, 320, TFT_DARKGREY);
	tft.drawFastHLine(0,  120, 320, TFT_DARKGREY);
	tft.drawFastHLine(0,  210, 320, TFT_DARKGREY);
}