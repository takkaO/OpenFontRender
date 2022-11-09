# Open Font Render [[JA](/doc/README_JA.md)]

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

render.setStartWrite(my_start_write_function); // optional
render.setEndWrite(my_end_write_function);     // optional
```

If you have an object for drawing and it contains the necessary methods, you can also use ```setDrawer``` method instead.

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
You may have to increase the stack size or enable useRenderTask to get it to work.

## Test

### Tested Hardware

This library has been tested on the following hardware.  
We would be happy to receive reports on hardware not listed here.

- Wio Terminal
- M5Stack Basic
- M5Stack Core2

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

## Reference

### FreeType

- [FreeType-2.11.0 API Reference](https://www.freetype.org/freetype2/docs/reference/index.html)
- [FreeType に関するメモ](http://ncl.sakura.ne.jp/doc/ja/comp/freetype-memo.html#ft-bitmap)
- [FreeType2 を試す-3-小さいサイズで描画 | ぬの部屋（仮）](https://suzulang.com/freetype2-3-small-size/)
- [True type font のレンダリング　その 2 - 忘備録-備忘録](https://blog.goo.ne.jp/lm324/e/9839bdff2c26272596c6ccebaafb78aa)
- [libvita2d/vita2d_font.c at master · xerpi/libvita2d](https://github.com/xerpi/libvita2d/blob/master/libvita2d/source/vita2d_font.c)
- [freetype 函数介绍\_guoke312 的专栏-程序员宅基地 - 程序员宅基地](https://www.cxyzjd.com/article/guoke312/79552120)
- [Freetype 内存回收 / 计算机图形/GUI/RTOS/FileSystem/OpenGL/DirectX/SDL2 / WhyCan Forum(哇酷开发者社区)](https://whycan.com/t_2984.html)
- [FreeType 2 Caching](https://www.sivachandran.in/2009/04/freetype-2-caching.html)
- [C++ (Cpp) FTC_Manager_LookupSize Examples - HotExamples](https://cpp.hotexamples.com/examples/-/-/FTC_Manager_LookupSize/cpp-ftc_manager_lookupsize-function-examples.html)
- [FreeType 的缓存 - 百度文库](https://wenku.baidu.com/view/046d34f90242a8956bece444.html)
- [freeType 移植总结 ①——使用 keil 编译 freeType2 库 - 编程猎人](https://www.programminghunter.com/article/6736670540/)
- [FreeType の使い方 - Qiita](https://qiita.com/496_/items/6d2540e26d66c9ed5f9a)
- [じぶんめも Qt で freetype](http://mochimocchi601.blog.fc2.com/blog-entry-4.html)
- [C++ (Cpp) FT_Glyph_To_Bitmap の例 - HotExamples](https://cpp.hotexamples.com/jp/examples/-/-/FT_Glyph_To_Bitmap/cpp-ft_glyph_to_bitmap-function-examples.html)
- [freetype2 - example1.c](https://www.freetype.org/freetype2/docs/tutorial/example1.c)
- [FreeType2 でフォントテクスチャ作る際のコツ。ピクセルサイズで作るのと文字送り量の取得 - GameProgrammar's Night](https://katze.hatenablog.jp/entry/2013/07/16/232911)
- [FreeType のベースラインあわせでかなりハマッタのでメモ - くそにそてくにっく](https://niso1985.hatenadiary.org/entry/20090907/1252339660)
- [【C++/MinGW】FreeTypeを使ってTTFファイルから文字レンダリング - とある科学の備忘録](https://shizenkarasuzon.hatenablog.com/entry/2020/10/30/181607)
- [OpenGLでFreeTypeを使って文字列を描画してみた２。(FreeTypeキャッシュ編)](https://gist.github.com/rg687076/76d4ddac2a8628066f201391f9a4489d)
- [FreeType を使って文字を描画する - インゲージ開発者ブログ](https://blog.ingage.jp/entry/2022/03/24/080000)
- [[Font][Freetype]26.6 fractional pixel formatってなぁに？　にお答えしますね – あおいろヨゾラ](https://www.nagatsuki-do.net/2014/05/18/fontfreetype26-6-fractional-pixel-format%E3%81%A3%E3%81%A6%E3%81%AA%E3%81%81%E3%81%AB%EF%BC%9F%E3%80%80%E3%81%AB%E3%81%8A%E7%AD%94%E3%81%88%E3%81%97%E3%81%BE%E3%81%99%E3%81%AD/)
- [freetype - How to get height of font in Freetype2 - Stack Overflow](https://stackoverflow.com/questions/50373457/how-to-get-height-of-font-in-freetype2)
- [True type fontのレンダリング　その7 - 忘備録-備忘録](https://blog.goo.ne.jp/lm324/e/457cf90b1c4267e1d49ed0881369b303)
- [FreeType-2.4.7 API Reference](https://opensource.apple.com/source/X11proto/X11proto-57.2/freetype/freetype-2.4.7/docs/reference/ft2-base_interface.html#FT_Open_Face)
- [The FreeType Caching Sub-System](http://www.fifi.org/doc/libfreetype6/cache.html)
- [freetype 2.8.1 | DirectAdmin Forums](https://forum.directadmin.com/threads/freetype-2-8-1.55304/)


### Other

- [Deleting a task and erasing it's handler - FreeRTOS](https://www.freertos.org/FreeRTOS_Support_Forum_Archive/October_2015/freertos_Deleting_a_task_and_erasing_its_handler_bb9bbcdbj.html)
- [FreeRTOS タスクスケジューリングまとめ - Qiita](https://qiita.com/MoriokaReimen/items/fe287a8bae1ce37849ae)
- [sed の正規表現で「\w」「\d」「\s」を実現する方法: 小粋空間](https://www.koikikukan.com/archives/2014/12/05-011111.php)
- [Regex in Powershell fails to check for newlines - Stack Overflow](https://stackoverflow.com/questions/52633995/regex-in-powershell-fails-to-check-for-newlines)

---

Portions of this software are copyright © The FreeTypeProject (www.freetype.org). All rights reserved.
