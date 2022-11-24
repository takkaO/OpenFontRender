# Open Font Render

Arduino IDEを用いたマイコン用のTTFフォントレンダリング支援ライブラリです。  
このライブラリは、SDカード内のTTFフォントファイルや、プログラムに埋め込まれたTTFフォントファイルを描画することができます。

このプログラムは [M5FontRender](https://github.com/takkaO/M5FontRender) がベースとなっています。  
**どんな些細な貢献も歓迎します**

![image](https://github.com/takkaO/OpenFontRender/blob/images/image.jpg)


## 特徴

- Arduino IDE を使用する様々なハードウェアで利用可能
- アンチエイリアスのかかった美しいフォントの描画
- コードに埋め込まれたフォントデータからの読み込みが可能（SDカード不要）
- 任意の描画ライブラリを使用可能
- FreeTypeの任意のバージョンに変更可能

## インストール

このリポジトリをArduinoのライブラリフォルダにクローンします。

## API と 使い方

### API

利用可能なAPIの一覧は[こちら](/doc/API.md)をご覧ください。

### 使い方

これは使い方の一部に過ぎません。  
より詳細な例は[examples](/examples/)にあります。

#### プログラムコードから読み込み (Wio Terminal)

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

#### SDカードから読み込み (M5Stack)

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

## TTFをプログラムに埋め込む方法

バイナリTTFフォントファイルを作成するために、toolsディレクトリにある[binary2ttf.py](/tools/ttf2bin/ttf2bin.py)を使用します。  
この `binary2ttf.py` は、[M5EPD](https://github.com/m5stack/M5EPD/tree/main/tools/ttf2bin)ライブラリで提供されています。  
同じプログラムが、このリポジトリの `tools` ディレクトリに含まれています。   
以下のコマンドを実行するだけです。

```sh
python3 binary2ttf.py your_font_file.ttf
```

## 描画関数の変更

このライブラリでは、描画関連の操作をお好みの関数（例えば LavyonGFX など）に変更することができます。  
変更するには、以下の関数を使用します。

```c++
render.setDrawPixel(my_draw_function);         // 必須

render.setDrawFastHLine(my_draw_fast_h_line);  // 任意
render.setStartWrite(my_start_write_function); // 任意
render.setEndWrite(my_end_write_function);     // 任意
```

描画用のオブジェクトがあり、必要なメソッドを含んでいる場合は `setDrawer` を利用することも出来ます。
より詳しい説明は [API.md](/doc/API.md) をご覧ください。

## FreeType のバージョンを変更する

このライブラリでは、どの FreeType バージョンにも切り替えることができます。  
2.4.12 と 2.12.1 で動作することを確認しています。  
デフォルトのバージョンは 2.4.12 ですが、これは最も安定して動作していたバージョンだったからです。

### 変更方法

1. お好きな FreeType のバージョンをダウンロードしてください。
2. ダウンロードした FreeType フォルダを OpenFontRender の直下に置きます。
3. `AutoRun` スクリプトを実行します。

### 備考

FreeRTOS を使用している場合、バージョンによっては不安定になることがあります。  
スタックサイズを大きくしたり、`useRenderTask` を有効にしないと動作しない場合があります。

## 動作検証

### 動作確認済みハードウェア

本ライブラリは，以下のハードウェアで動作確認をしています。  
ここに掲載されていないハードウェアでの報告もお待ちしております。

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

### 描画速度

下の表はサンプルプログラムを実行したときの描画時間です。  
注：描画時間はハードウェアの設定やフォントファイルによって異なるので、参考程度にお考えください。

|               | Load from SD | Load from array | Load from array <br> (Use LavyonGFX) |
| ------------- | :----------: | :-------------: | :----------------------------------: |
| Wio Terminal  |    576 ms    |     491 ms      |                405 ms                |
| M5Stack Basic |    227 ms    |     230 ms      |                98 ms                 |
| M5Stack Core2 |    346 ms    |     281 ms      |                148 ms                |

## ライセンス

このライブラリは FTL ライセンスで提供されています。  
これは、クレジット条項を持つ BSD スタイルのライセンスです。  
本ライブラリを使用して作成されたプログラムやバイナリには，必ずクレジットを記載しなければなりません（本READMEの下部に記載されているようなものです）。

ただし，FreeType に関係の無いいくつかのファイルは，MIT ライセンスのもとで使用することができます（各ファイルを参照）．

FTL ライセンスの詳細については，このリポジトリの [doc/FTL.TXT](/doc/FTL.TXT) または [FreeType Licenses](https://www.freetype.org/license.html) を参照してください．

### サンプルフォント

サンプルプログラムには、以下のフォントファイルを使用しています。  
これらの素晴らしいフォントを使いやすいライセンスで提供してくださった sozai-font-hako さんに感謝いたします。

| Font | Copyright | 
| --- | --- | 
|[JK Gothic M](http://font.cutegirl.jp/jk-font-medium.html#i)|Copyright  (c) 2014 M+ FONTS PROJECT <br> Copyright  (c) 2015 JK FONTS|

## 参考文献

### FreeType

- [FreeType-2.11.0 API Reference](https://www.freetype.org/freetype2/docs/reference/index.html)
- [FreeType に関するメモ](http://ncl.sakura.ne.jp/doc/ja/comp/freetype-memo.html#ft-bitmap)
- [FreeType2を試す-3-小さいサイズで描画 | ぬの部屋（仮）](https://suzulang.com/freetype2-3-small-size/)
- [True type fontのレンダリング　その2 - 忘備録-備忘録](https://blog.goo.ne.jp/lm324/e/9839bdff2c26272596c6ccebaafb78aa)
- [libvita2d/vita2d_font.c at master · xerpi/libvita2d](https://github.com/xerpi/libvita2d/blob/master/libvita2d/source/vita2d_font.c)
- [freetype 函数介绍_guoke312的专栏-程序员宅基地 - 程序员宅基地](https://www.cxyzjd.com/article/guoke312/79552120)
- [Freetype 内存回收 / 计算机图形/GUI/RTOS/FileSystem/OpenGL/DirectX/SDL2 / WhyCan Forum(哇酷开发者社区)](https://whycan.com/t_2984.html)
- [FreeType 2 Caching](https://www.sivachandran.in/2009/04/freetype-2-caching.html)
- [C++ (Cpp) FTC_Manager_LookupSize Examples - HotExamples](https://cpp.hotexamples.com/examples/-/-/FTC_Manager_LookupSize/cpp-ftc_manager_lookupsize-function-examples.html)
- [FreeType的缓存 - 百度文库](https://wenku.baidu.com/view/046d34f90242a8956bece444.html)
- [freeType移植总结①——使用keil编译freeType2库 - 编程猎人](https://www.programminghunter.com/article/6736670540/)
- [FreeTypeの使い方 - Qiita](https://qiita.com/496_/items/6d2540e26d66c9ed5f9a)
- [じぶんめも Qtでfreetype](http://mochimocchi601.blog.fc2.com/blog-entry-4.html)
- [C++ (Cpp) FT_Glyph_To_Bitmapの例 - HotExamples](https://cpp.hotexamples.com/jp/examples/-/-/FT_Glyph_To_Bitmap/cpp-ft_glyph_to_bitmap-function-examples.html)
- [freetype2 - example1.c](https://www.freetype.org/freetype2/docs/tutorial/example1.c)
- [FreeType2でフォントテクスチャ作る際のコツ。ピクセルサイズで作るのと文字送り量の取得 - GameProgrammar's Night](https://katze.hatenablog.jp/entry/2013/07/16/232911)
- [FreeTypeのベースラインあわせでかなりハマッタのでメモ - くそにそてくにっく](https://niso1985.hatenadiary.org/entry/20090907/1252339660)
- [【C++/MinGW】FreeTypeを使ってTTFファイルから文字レンダリング - とある科学の備忘録](https://shizenkarasuzon.hatenablog.com/entry/2020/10/30/181607)
- [OpenGLでFreeTypeを使って文字列を描画してみた２。(FreeTypeキャッシュ編)](https://gist.github.com/rg687076/76d4ddac2a8628066f201391f9a4489d)
- [FreeType を使って文字を描画する - インゲージ開発者ブログ](https://blog.ingage.jp/entry/2022/03/24/080000)
- [[Font][Freetype]26.6 fractional pixel formatってなぁに？　にお答えしますね – あおいろヨゾラ](https://www.nagatsuki-do.net/2014/05/18/fontfreetype26-6-fractional-pixel-format%E3%81%A3%E3%81%A6%E3%81%AA%E3%81%81%E3%81%AB%EF%BC%9F%E3%80%80%E3%81%AB%E3%81%8A%E7%AD%94%E3%81%88%E3%81%97%E3%81%BE%E3%81%99%E3%81%AD/)
- [freetype - How to get height of font in Freetype2 - Stack Overflow](https://stackoverflow.com/questions/50373457/how-to-get-height-of-font-in-freetype2)
- [True type fontのレンダリング　その7 - 忘備録-備忘録](https://blog.goo.ne.jp/lm324/e/457cf90b1c4267e1d49ed0881369b303)
- [FreeType-2.4.7 API Reference](https://opensource.apple.com/source/X11proto/X11proto-57.2/freetype/freetype-2.4.7/docs/reference/ft2-base_interface.html#FT_Open_Face)
- [The FreeType Caching Sub-System](http://www.fifi.org/doc/libfreetype6/cache.html)
- [freetype 2.8.1 | DirectAdmin Forums](https://forum.directadmin.com/threads/freetype-2-8-1.55304/)

### その他

- [Deleting a task and erasing it's handler - FreeRTOS](https://www.freertos.org/FreeRTOS_Support_Forum_Archive/October_2015/freertos_Deleting_a_task_and_erasing_its_handler_bb9bbcdbj.html)
- [FreeRTOS タスクスケジューリングまとめ - Qiita](https://qiita.com/MoriokaReimen/items/fe287a8bae1ce37849ae)
- [sedの正規表現で「\w」「\d」「\s」を実現する方法: 小粋空間](https://www.koikikukan.com/archives/2014/12/05-011111.php)
- [Regex in Powershell fails to check for newlines - Stack Overflow](https://stackoverflow.com/questions/52633995/regex-in-powershell-fails-to-check-for-newlines)

---

Portions of this software are copyright © The FreeTypeProject (www.freetype.org). All rights reserved.
