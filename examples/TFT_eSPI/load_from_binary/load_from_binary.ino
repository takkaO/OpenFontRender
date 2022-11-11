/*
 Font generation:

 This sketch uses a ttf (TrueType Font) file that has been converted to a binary
 format in a byte array. See NotoSans_Bold.h tab of this sketch.

    https://en.wikipedia.org/wiki/TrueType

 The font used in this sketch is free to use and from Google:

    https://fonts.google.com/
 
 TTF font files can be VERY large, fortunately there are python and online tools that can
 be used to convert a ttf font to a new ttf font file with a subset of the characters
 (called "font subsetting") as needed by the sketch. For example maybe only 0-9 and : are
 needed to display the time. It’s important to check the licence a particular font before
 use and subsetting. Here is a simple subsetting online tool:

    https://products.aspose.app/font/generator/ttf-to-ttf

 To use this tool:
   1. Drag and drop file
   2. Copy and paste the "Font symbols" you want, here are ASCII characters 20 (space) and 33-126:
       !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
   3. Make sure you include the "space" character if you are going to use it (renders as a box otherwise)
   4. Click "Generate"
   5. Save the file and convert to an array

 There is a more sophisticated font subsetting tool here:

    https://www.fontsquirrel.com/tools/webfont-generator

 The font must be in ttf format, it can then be converted to a binary array using the tool here:
    
    https://notisrac.github.io/FileToCArray/

 To use this tool:
   1. Drag and drop the ttf file on "Browse..." button
   2. Untick box "static"
   3. Click "Convert"
   4. Click "Save as file" and move the header file to sketch folder
   5. Open the sketch in IDE
   6. Include the header file containing the array (NotoSans_Bold.h in this example)

 ttf font files can be very large, there are python and online tools that can
 be used to convert a ttf font to a new font file with a subset of the characters
 that will be used by the sketch. For example maybe only 0-9 and : are needed to
 display the time.

 Note that translating a ttf font character to a glyph that can be rendered on
 screen is a processor intensive operation. This means the rendering speed will
 be much lower than bitmap encoded fonts which just need to be copied to the
 display.

 TrueType font characters are scalable, so different font files are not needed to
 render different size characters on the TFT screen. This is very convenient, just
 use the setFontSize(font_size) function (see below).
*/

#include "TFT_eSPI.h"
#include "binaryttf.h"
#include "OpenFontRender.h"

TFT_eSPI tft;
OpenFontRender render;

void setup() {
  Serial.begin(115200);

  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLUE);

  if (render.loadFont(binaryttf, sizeof(binaryttf))) {
    Serial.println("Render initialize error");
    return;
  }
  render.setDrawer(tft); // Set drawer object

  render.setFontColor(TFT_WHITE, TFT_BLUE);
  render.printf("Hello World\n");
  render.seekCursor(0, 10);

  render.setFontSize(30);
  render.setFontColor(TFT_GREEN, TFT_BLUE);
  render.printf("完全なUnicodeサポート\n");
  render.seekCursor(0, 10);

  render.setFontSize(40);
  render.setFontColor(TFT_ORANGE, TFT_BLUE);
  render.printf("こんにちは世界\n");
}

void loop() {
}
