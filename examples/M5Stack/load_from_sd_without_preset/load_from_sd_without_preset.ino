#include <Arduino.h>
#include <M5Unified.h>
#include <SD.h>

#include "OpenFontRender.h" // Include after M5Unified.h

OpenFontRender render;

/*=== Overwrite SD (File) operation methods BEGIN ===*/
File myFile;

FT_FILE *OFR_fopen(const char *filename, const char *mode)
{
	myFile = SD.open(filename, mode);
	return &myFile;
}

void OFR_fclose(FT_FILE *stream)
{
	// myFile.close()
	((File *)stream)->close();
}

size_t OFR_fread(void *ptr, size_t size, size_t nmemb, FT_FILE *stream)
{
	// myFile.read()
	return ((File *)stream)->read((uint8_t *)ptr, size * nmemb);
}

int OFR_fseek(FT_FILE *stream, long int offset, int whence)
{
	// myFile.seek()
	return ((File *)stream)->seek(offset, (SeekMode)whence);
}

long int OFR_ftell(FT_FILE *stream)
{
	// myFile.position()
	return ((File *)stream)->position();
}
/*=== Overwrite SD (File) operation methods END ===*/

void setup()
{
	// put your setup code here, to run once:
	auto cfg = M5.config();
	M5.begin(cfg);
	M5.Display.fillScreen(TFT_BLACK);

	SD.begin(GPIO_NUM_4, SPI, 40000000);

	render.setSerial(Serial);	  // Need to print render library message
	render.showFreeTypeVersion(); // print FreeType version
	render.showCredit();		  // print FTL credit

	if (render.loadFont("/JKG-M_3_Tiny.ttf"))
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
