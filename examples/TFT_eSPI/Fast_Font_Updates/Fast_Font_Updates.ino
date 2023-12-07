//////////////////////////////////////////////////////////////////////
// This example renders millis() to a sprite in the middle of the
// screen, and then draws over it with a new sprite. This should
// have the effect of smooth screen updates without any flickering
// or font tearing.
//
// Using this method I can get 100+ FPS on an ESP32 with a size 64
// font.
//////////////////////////////////////////////////////////////////////

#include "OpenFontRender.h"
OpenFontRender ofr;

#include "NotoSans_Bold.h"
#define TTF_FONT NotoSans_Bold

#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft       = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

void setup(void) {
	tft.begin();
	tft.setRotation(1);
	tft.fillScreen(TFT_BLACK);

	ofr.setDrawer(sprite);
	ofr.loadFont(TTF_FONT, sizeof(TTF_FONT));
}

void loop() {
	// Build the sprite and fill it with black
	void *out = sprite.createSprite(140, 50);
	if (out == NULL) Serial.println("Could not allocate memory for sprite!");
	sprite.fillSprite(TFT_BLACK);

	// Set all the font settings and write millis() to the sprite
	ofr.setFontSize(64);
	ofr.setCursor(sprite.width() / 2, sprite.height() / 2); // Middle of box
	ofr.setAlignment(Align::MiddleCenter);
	ofr.setFontColor(TFT_WHITE, TFT_BLACK);
	ofr.printf("%d", millis());

	// Calculate the offsets needed to center the sprite box
	int hcenter = (tft.width()  / 2) - (sprite.width()  / 2);
	int vcenter = (tft.height() / 2) - (sprite.height() / 2);

	// Push the sprite to the screen
	sprite.pushSprite(hcenter,vcenter);
}
