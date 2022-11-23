# Open Font Render \[[JA](/doc/README_JA.md)\]

TTF font render support library for microcomputer using Arduino IDE.  
This library can render TTF font files in the SD card or TTF font files embedded in the program.

This program is inspired by [M5FontRender](https://github.com/takkaO/M5FontRender).  
**Any small contribution is welcome.**

![image](https://github.com/takkaO/OpenFontRender/blob/images/image.jpg)

## Features

- Available for a variety of hardware using the Arduino IDE
- Draws beautiful, anti-aliased fonts
- Can be loaded from font data embedded in code (no SD card required)
- Can use any drawing library
- Can be changed to any version of FreeType.

## Installation

Clone this repository into Arduino library folder.

## API and Usage

### API

See [HERE](/doc/API.md) for a list of available APIs.

### Usage

This is only a part of the usage.  
More detailed examples can be found in [examples](/examples/).

#### Load from array (Wio Terminal)

```c++
#include "TFT_eSPI.h"
#include "binaryttf.h"
#include "OpenFontRender.h"

TFT_eSPI tft;
OpenFontRender render;

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);
	Serial.flush();
	delay(50);

	tft.begin();
	tft.setRotation(3);
	tft.fillScreen(TFT_BLACK);
	digitalWrite(LCD_BACKLIGHT, HIGH); // turn on the backlight

	// print library version
	Serial.printf("OpenFontRender Library version: %d.%d\n",
	              OpenFontRender::MAIN_VERSION, 
	              OpenFontRender::MINER_VERSION);

	render.showFreeTypeVersion(); // print FreeType version
	render.showCredit();		  // print FTL credit

	if (render.loadFont(binaryttf, sizeof(binaryttf))) {
		Serial.println("Render initialize error");
		return;
	}
	render.setDrawer(tft); // Set drawer object

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
}

void loop() {
	// put your main code here, to run repeatedly:
}
```

#### Load from SD card (M5Stack)

```c++
#if defined(ARDUINO_M5Stack_Core_ESP32) || defined(ARDUINO_M5STACK_FIRE)
#include "M5Stack.h"
#elif defined(ARDUINO_M5STACK_Core2) // M5Stack Core2
#include "M5Core2.h"
#endif

#include "OpenFontRender.h" // Include after M5Stack.h / M5Core2.h

OpenFontRender render;

void setup() {
	// put your setup code here, to run once:
	M5.begin();
	M5.Lcd.fillScreen(BLACK);

	// print library version
	Serial.printf("OpenFontRender Library version: %d.%d\n",
	              OpenFontRender::MAIN_VERSION, 
	              OpenFontRender::MINER_VERSION);

	render.showFreeTypeVersion(); // print FreeType version
	render.showCredit();		  // print FTL credit

	if (render.loadFont("/JKG-M_3_Tiny.ttf")) 	{
		Serial.println("Render initialize error");
		return;
	}
	render.setDrawer(M5.Lcd); // Set drawer object

	render.setFontColor(WHITE);
	render.printf("Hello World\n");
	render.seekCursor(0, 10);

	render.setFontSize(30);
	render.setFontColor(GREEN);
	render.printf("完全なUnicodeサポート\n");
	render.seekCursor(0, 10);

	render.setFontSize(40);
	render.setFontColor(ORANGE);
	render.printf("こんにちは世界\n");
}

void loop() {
	// put your main code here, to run repeatedly:
}
```

## How to create binary TTF file

We use [binary2ttf.py](/tools/ttf2bin/ttf2bin.py) in tools directory to create binary TTF font file.  
The `binary2ttf.py` is provided in the [M5EPD](https://github.com/m5stack/M5EPD/tree/main/tools/ttf2bin) library.
The same program is included in `tools` directory in this repo.  
You only execute below command.

```sh
python3 binary2ttf.py your_font_file.ttf
```

## Switch draw function

In this library, you can change the drawing-related operations to your favorite functions (e.g. LavyonGFX).  
To change it, use the following function.

```c++
render.setDrawPixel(my_draw_function);         // necessary

render.setDrawFastHLine(my_draw_fast_h_line);  // optional
render.setStartWrite(my_start_write_function); // optional
render.setEndWrite(my_end_write_function);     // optional
```

If you have an object for drawing and it contains the necessary methods, you can also use `setDrawer` method instead.

See [API.md](/doc/API.md) for more detailed instructions.

## Switch other FreeType version

You can switch to any FreeType version in this library.  
We have confirmed that it works with 2.4.12 and 2.12.1.  
Default version is 2.4.12 because it was the version that worked most stably.

### How to switch

1. Download the version of FreeType that you like.
2. Place the downloaded FreeType folder directly under OpenFontRender.
3. Execute `AutoRun` script.

### Note

If you are using FreeRTOS, some versions may become unstable.  
You may have to increase the stack size or enable `useRenderTask` to get it to work.

## Test

### Tested Hardware

This library has been tested on the following hardware.  
We would be happy to receive reports on hardware not listed here.

- Arduino IDE 2.0
  - Seeed SAMD Boards (Board Manager v1.8.3)
    - Wio Terminal
  - M5Stack (Board Manager v2.0.4)
    - M5Stack Basic (Library v0.1.4)
    - M5Stack Core2 (Library v0.4.0)
- PlatformIO
  - M5Stack Basic (Library v2.0.4)
  - M5Stack Core2 (Library v2.0.4)
- ESP-IDF (4.4)
  - M5Stack Basic

### Rendering speed

The table below shows the time taken to draw the sample program when it was run.
Note: the time depends on the hardware settings or font file, so this is for reference only.

|               | Load from SD | Load from array | Load from array <br> (Use LavyonGFX) |
| ------------- | :----------: | :-------------: | :----------------------------------: |
| Wio Terminal  |    576 ms    |     491 ms      |                405 ms                |
| M5Stack Basic |    227 ms    |     230 ms      |                98 ms                 |
| M5Stack Core2 |    346 ms    |     281 ms      |                148 ms                |

## LICENSE

This library is provided under the FTL license.  
It is a BSD-style license with a credit clause.  
All programs and binaries created using this library must be credited (as shown at the bottom of this README).

However, some files that are not related to FreeType can be used under the MIT license (See each files).

For more information about FTL licenses, see [doc/FTL.TXT](/doc/FTL.TXT) in this repository or [FreeType Licenses](https://www.freetype.org/license.html).

### Sample font

We have used below font file for the sample program.  
We would like to thank sozai-font-hako for providing us with an easy-to-use license for these wonderful fonts.

| Font                                                         | Copyright                                                            |
| ------------------------------------------------------------ | -------------------------------------------------------------------- |
| [JK Gothic M](http://font.cutegirl.jp/jk-font-medium.html#i) | Copyright (c) 2014 M+ FONTS PROJECT <br> Copyright (c) 2015 JK FONTS |

---

Portions of this software are copyright © The FreeTypeProject (www.freetype.org). All rights reserved.
