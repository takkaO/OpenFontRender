# API

---

## Font loading API

---

```c++
FT_Error loadFont(const unsigned char *data, size_t size)
```

### Summary

Reads the font data embedded in the program code.

### Arguments

| Type                   | Name | Description   |
| ---------------------- | :--: | ------------- |
| const unsigned char \* | data | Array pointer |
| size_t                 | size | Array size    |

### Return

| Type     | Description                        |
| -------- | ---------------------------------- |
| FT_Error | FreeType error code. 0 is success. |

---

```c++
FT_Error loadFont(const char *fpath)
```

### Summary

Load font data from external storage (SD card).

### Arguments

| Type          | Name  | Description |
| ------------- | :---: | ----------- |
| const char \* | fpath | File path   |

### Return

| Type     | Description                        |
| -------- | ---------------------------------- |
| FT_Error | FreeType error code. 0 is success. |

### Note

**SD card access is strongly hardware dependent, so for hardware other than M5Stack and Wio Terminal, you will need to add file manipulation functions to FileSupport.cpp/.h.**  
Any better solutions are welcome.

---

```c++
void unloadFont()
```

### Summary

Unload font data.

### Arguments

None

### Return

None

---

## Rendering API

---

```c++
FT_Error drawChar(char character,
                  uint32_t x  = 0,
                  uint32_t y  = 0,
                  uint16_t fg = 0xFFFF,
                  uint16_t bg = 0x0000,
                  Align align = Align::Left)
```

### Summary

Draws a single character.

### Arguments

| Type     |   Name    | Description                            |
| -------- | :-------: | -------------------------------------- |
| char     | character | Character to draw                      |
| uint32_t |     x     | Top-left X coordinate to start drawing |
| uint32_t |     y     | Top-left Y coordinate to start drawing |
| uint32_t |    fg     | Font color (16bit color)               |
| uint32_t |    bg     | Font background color (16bit color)    |
| Align    |   align   | Alignment of character                 |

### Return

| Type     | Description                        |
| -------- | ---------------------------------- |
| FT_Error | FreeType error code. 0 is success. |

---

```c++
uint16_t drawHString(const char *str,
                    uint32_t x,
                    uint32_t y,
                    uint16_t fg,
                    uint16_t bg,
                    Align align,
                    Drawing drawing,
                    FT_BBox &abbox,
                    FT_Error &error)
```

### Summary

Draw strings horizontally.

### Arguments

| Type          |  Name   | Description                                                |
| ------------- | :-----: | ---------------------------------------------------------- |
| const char \* |   str   | String to draw                                             |
| uint32_t      |    x    | Top-left X coordinate to start drawing                     |
| uint32_t      |    y    | Top-left Y coordinate to start drawing                     |
| uint32_t      |   fg    | Font color (16bit color)                                   |
| uint32_t      |   bg    | Font background color (16bit color)                        |
| Align         |  align  | Alignment of strings                                       |
| Drawing       | drawing | Whether to draw or not                                     |
| FT_BBox       | &abbox  | (output) Smallest rectangle that encloses the drawn string |
| FT_Error      | &error  | (output) FreeType error code. 0 is success                 |

### Return

| Type     | Description                            |
| -------- | -------------------------------------- |
| uint16_t | Number of characters success to write. |

---

```c++
uint16_t drawString(const char *str,
                    uint32_t x  = 0,
                    uint32_t y  = 0,
                    uint16_t fg = 0xFFFF,
                    uint16_t bg = 0x0000,
                    Layout layout = Layout::Horizontal)
```

### Summary

Draws the specified string left-aligned.

### Arguments

| Type          |  Name  | Description                                              |
| ------------- | :----: | -------------------------------------------------------- |
| const char \* |  str   | String to draw                                           |
| uint32_t      |   x    | Top-left X coordinate to start drawing                   |
| uint32_t      |   y    | Top-left Y coordinate to start drawing                   |
| uint32_t      |   fg   | Font color (16bit color)                                 |
| uint32_t      |   bg   | Font background color (16bit color)                      |
| Layout        | layout | String drawing direction (Vertical is NOT supported now) |

### Return

| Type     | Description                            |
| -------- | -------------------------------------- |
| uint16_t | Number of characters success to write. |

### Note

Vertical writing is currently not supported, but is available for future expansion.

---

```c++
uint16_t cdrawString(const char *str,
                    uint32_t x  = 0,
                    uint32_t y  = 0,
                    uint16_t fg = 0xFFFF,
                    uint16_t bg = 0x0000,
                    Layout layout = Layout::Horizontal)
```

### Summary

Draws the specified string centered.

### Arguments

| Type          |  Name  | Description                                              |
| ------------- | :----: | -------------------------------------------------------- |
| const char \* |  str   | String to draw                                           |
| uint32_t      |   x    | Top-center X coordinate to start drawing                 |
| uint32_t      |   y    | Top-center Y coordinate to start drawing                 |
| uint32_t      |   fg   | Font color (16bit color)                                 |
| uint32_t      |   bg   | Font background color (16bit color)                      |
| Layout        | layout | String drawing direction (Vertical is NOT supported now) |

### Return

| Type     | Description                            |
| -------- | -------------------------------------- |
| uint16_t | Number of characters success to write. |

### Note

Vertical writing is currently not supported, but is available for future expansion.

---

```c++
uint16_t rdrawString(const char *str,
                    uint32_t x  = 0,
                    uint32_t y  = 0,
                    uint16_t fg = 0xFFFF,
                    uint16_t bg = 0x0000,
                    Layout layout = Layout::Horizontal)
```

### Summary

Draws the specified string right-aligned.

### Arguments

| Type          |  Name  | Description                                              |
| ------------- | :----: | -------------------------------------------------------- |
| const char \* |  str   | String to draw                                           |
| uint32_t      |   x    | Top-right X coordinate to start drawing                  |
| uint32_t      |   y    | Top-right Y coordinate to start drawing                  |
| uint32_t      |   fg   | Font color (16bit color)                                 |
| uint32_t      |   bg   | Font background color (16bit color)                      |
| Layout        | layout | String drawing direction (Vertical is NOT supported now) |

### Return

| Type     | Description                            |
| -------- | -------------------------------------- |
| uint16_t | Number of characters success to write. |

### Note

Vertical writing is currently not supported, but is available for future expansion.

---

```c++
uint16_t printf(const char *fmt, ...)
```

### Summary

Draws the specified format string left-aligned.

### Arguments

| Type          | Name | Description   |
| ------------- | :--: | ------------- |
| const char \* | fmt  | Format string |

### Return

| Type     | Description                            |
| -------- | -------------------------------------- |
| uint16_t | Number of characters success to write. |

---

```c++
uint16_t cprintf(const char *fmt, ...)
```

### Summary

Draws the specified format string centered.

### Arguments

| Type          | Name | Description   |
| ------------- | :--: | ------------- |
| const char \* | fmt  | Format string |

### Return

| Type     | Description                            |
| -------- | -------------------------------------- |
| uint16_t | Number of characters success to write. |

---

```c++
uint16_t rprintf(const char *fmt, ...)
```

### Summary

Draws the specified format string right-aligned.

### Arguments

| Type          | Name | Description   |
| ------------- | :--: | ------------- |
| const char \* | fmt  | Format string |

### Return

| Type     | Description                            |
| -------- | -------------------------------------- |
| uint16_t | Number of characters success to write. |

---

```c++
FT_BBox calculateBoundingBoxFmt(int32_t x,
                                 int32_t y,
                                 unsigned int font_size,
                                 Align align,
                                 Layout layout,
                                 const char *fmt, ...)
```

### Summary

Obtains the enclosing rectangle of a string when drawn under the specified conditions.

### Arguments

| Type          |   Name    | Description                                              |
| ------------- | :-------: | -------------------------------------------------------- |
| int32_t       |     x     | X coordinate to start drawing                            |
| int32_t       |     y     | Y coordinate to start drawing                            |
| unsigned int  | font_size | Drawing font size                                        |
| Align         |   align   | Alignment of strings                                     |
| Layout        |  layout   | String drawing direction (Vertical is NOT supported now) |
| const char \* |    fmt    | Format string                                            |

### Return

| Type    | Description                                        |
| ------- | -------------------------------------------------- |
| FT_BBox | Smallest rectangle that encloses the drawn string. |

---

```c++
FT_BBox calculateBoundingBox(int32_t x,
                                 int32_t y,
                                 unsigned int font_size,
                                 Align align,
                                 Layout layout,
                                 const char *str)
```

### Summary

Obtains the enclosing rectangle of a string when drawn under the specified conditions.

### Arguments

| Type          |   Name    | Description                                              |
| ------------- | :-------: | -------------------------------------------------------- |
| int32_t       |     x     | X coordinate to start drawing                            |
| int32_t       |     y     | Y coordinate to start drawing                            |
| unsigned int  | font_size | Drawing font size                                        |
| Align         |   align   | Alignment of strings                                     |
| Layout        |  layout   | String drawing direction (Vertical is NOT supported now) |
| const char \* |    str    | Drawing string                                           |

### Return

| Type    | Description                                        |
| ------- | -------------------------------------------------- |
| FT_BBox | Smallest rectangle that encloses the drawn string. |

---

```c++
unsigned int calculateFitFontSizeFmt(uint32_t limit_width,
                                     uint32_t limit_height,
                                     Layout layout,
                                     const char *fmt, ...)
```

### Summary

Calculates the maximum font size that will fit in the specified rectangle.

### Arguments

| Type          |     Name     | Description                                              |
| ------------- | :----------: | -------------------------------------------------------- |
| uint32_t      | limit_width  | Rectangle width limit                                    |
| uint32_t      | limit_height | Rectangle height limit                                   |
| Layout        |    layout    | String drawing direction (Vertical is NOT supported now) |
| const char \* |     fmt      | Format string                                            |

### Return

| Type         | Description           |
| ------------ | --------------------- |
| unsigned int | Calculated font size. |

### Note

Note that the calculated font size may not exactly meet the size limit.
This is because the font size is not strictly measured, but only "estimated" by calculation.

---

```c++
unsigned int calculateFitFontSize(uint32_t limit_width,
                                     uint32_t limit_height,
                                     Layout layout,
                                     const char *str)
```

### Summary

Calculates the maximum font size that will fit in the specified rectangle.

### Arguments

| Type          |     Name     | Description                                              |
| ------------- | :----------: | -------------------------------------------------------- |
| uint32_t      | limit_width  | Rectangle width limit                                    |
| uint32_t      | limit_height | Rectangle height limit                                   |
| Layout        |    layout    | String drawing direction (Vertical is NOT supported now) |
| const char \* |     str      | Drawing string                                           |

### Return

| Type         | Description           |
| ------------ | --------------------- |
| unsigned int | Calculated font size. |

### Note

Note that the calculated font size may not exactly meet the size limit.
This is because the font size is not strictly measured, but only "estimated" by calculation.

---

```c++
void setDrawPixel(Function f)
```

### Summary

Set the function for drawing on the LCD.

### Arguments

| Type     | Name | Description         |
| -------- | :--: | ------------------- |
| Function |  f   | Draw pixel function |

### Return

None

### Note

The function to be given must be a function like the one below that takes a specific argument.

```c++
void example_function (int32_t x, int32_t y, uint16_t c)
```

| Type    | Name | Description               |
| ------- | :--: | ------------------------- |
| int32_t |  x   | Draw position X           |
| int32_t |  y   | Draw position Y           |
| int32_t |  c   | Draw color (16 bit color) |

---

```c++
void setStartWrite(Function f)
```

### Summary

It is called only once at the beginning of a sequence of drawings.  
Certain libraries can occupy the bus during continuous drawing to increase the drawing speed.

### Arguments

| Type     | Name | Description   |
| -------- | :--: | ------------- |
| Function |  f   | user function |

### Return

None

### Note

The function to be given must be a function like the one below that takes a specific argument.

```c++
void example_function (void)
```

---

```c++
void setEndWrite(Function f)
```

### Summary

It is called only once at the end of a sequence of drawings.  
Certain libraries can occupy the bus during continuous drawing to increase the drawing speed.

### Arguments

| Type     | Name | Description   |
| -------- | :--: | ------------- |
| Function |  f   | user function |

### Return

None

### Note

The function to be given must be a function like the one below that takes a specific argument.

```c++
void example_function (void)
```

---

```c++
template <typename T> void setDrawer(T &drawer)
```

### Summary

If any object drawer has `drawPixel`, `startWrite`, and `endWrite` methods, you can use this function to automatically call `setDrawPixel`, `startWrite`, and `endWrite`.

### Arguments

| Type |  Name  | Description |
| ---- | :----: | ----------- |
| Any  | drawer | user object |

### Return

None

### Note

Inside the function, the following process takes place.

```c++
template <typename T> void setDrawer(T &drawer) {
    setDrawPixel(drawer.drawPixel);
    setStartWrite(drawer.startWrite);
    setEndWrite(drawer.endWrite);
}
```

---

## FreeRTOS-related API

---

```c++
void setUseRenderTask(bool enable)
```

### Summary

Set whether or not to create a render task.  
This is meaningless if you are not using FreeRTOS.
Default is `false`.

### Arguments

| Type |  Name  | Description                      |
| ---- | :----: | -------------------------------- |
| bool | enable | Enable (true) or Disable (false) |

### Return

None

### Note

If you NOT use FreeType 2.4.12, recommend to set `enable`.

---

```c++
void setRenderTaskStackSize(unsigned int stack_size)
```

### Summary

Set stack size for render task;

### Arguments

| Type         |    Name    | Description            |
| ------------ | :--------: | ---------------------- |
| unsigned int | stack_size | render task stack size |

### Return

None

### Note

If you use this method, please call before first call of rendering method.

---

## Cursor operation API

---

```c++
void setCursor(int32_t x, int32_t y)
```

### Summary

Set the position of the internal cursor held by the renderer.  
It is used when drawing with the `printf` function.

### Arguments

| Type    | Name | Description         |
| ------- | :--: | ------------------- |
| int32_t |  x   | X coordinate to set |
| int32_t |  y   | Y coordinate to set |

### Return

None

---

```c++
int32_t getCursorX()
```

### Summary

Get the X coordinate of the internal cursor held by the renderer.

### Arguments

None

### Return

| Type    | Description          |
| ------- | -------------------- |
| int32_t | Current X coordinate |

---

```c++
int32_t getCursorY()
```

### Summary

Get the Y coordinate of the internal cursor held by the renderer.

### Arguments

None

### Return

| Type    | Description          |
| ------- | -------------------- |
| int32_t | Current Y coordinate |

---

```c++
void seekCursor(int32_t delta_x, int32_t delta_y)
```

### Summary

Shifts the position of the internal cursor held by the renderer by the specified amount.  
This is useful for making fine adjustments.

### Arguments

| Type    |  Name   | Description          |
| ------- | :-----: | -------------------- |
| int32_t | delta_x | X coordinate to move |
| int32_t | delta_y | Y coordinate to move |

### Return

None

---

## Font-related API

---

```c++
void setFontColor(uint16_t font_color)
```

### Summary

Set the font color to be retained by the renderer.  
It is used when drawing with the `printf` function.

### Arguments

| Type     |    Name    | Description                      |
| -------- | :--------: | -------------------------------- |
| uint16_t | font_color | Font color to set (16 bit color) |

### Return

None

---

```c++
void setFontColor(uint16_t font_color, uint16_t font_bgcolor)
```

### Summary

Set the font color and background color to be retained by the renderer.  
It is used when drawing with the `printf` function.

### Arguments

| Type     |     Name     | Description                                 |
| -------- | :----------: | ------------------------------------------- |
| uint16_t |  font_color  | Font color to set (16 bit color)            |
| uint16_t | font_bgcolor | Font background color to set (16 bit color) |

### Return

None

---

```c++
void setFontColor(uint8_t r, uint8_t g, uint8_t b)
```

### Summary

Set the font color to be retained by the renderer.  
It is used when drawing with the `printf` function.

### Arguments

| Type    | Name | Description       |
| ------- | :--: | ----------------- |
| uint8_t |  r   | Red color value   |
| uint8_t |  g   | Green color value |
| uint8_t |  b   | Blue color value  |

### Return

None

---

```c++
void setFontColor(uint8_t fr,
                  uint8_t fg,
                  uint8_t fb,
                  uint8_t br,
                  uint8_t bg,
                  uint8_t bb)
```

### Summary

Set the font color and background color to be retained by the renderer.
It is used when drawing with the `printf` function.

### Arguments

| Type    | Name | Description                      |
| ------- | :--: | -------------------------------- |
| uint8_t |  fr  | Red color value for font         |
| uint8_t |  fg  | Green color value for font       |
| uint8_t |  fb  | Blue color value for font        |
| uint8_t |  br  | Red color value for background   |
| uint8_t |  bg  | Green color value for background |
| uint8_t |  bb  | Blue color value for background  |

### Return

None

---

```c++
void setBackgroundColor(uint16_t font_bgcolor)
```

### Summary

Set the font background color to be retained by the renderer.  
It is used when drawing with the `printf` function.

### Arguments

| Type     |     Name     | Description                                 |
| -------- | :----------: | ------------------------------------------- |
| uint16_t | font_bgcolor | Font background color to set (16 bit color) |

### Return

None

---

```c++
void setTransparentBackground(bool enable)
```

### Summary

The text background is transparent (i.e., not drawn).
However, if the background colors do not match, an incongruity may appear near the text border.

### Arguments

| Type |  Name  | Description                      |
| ---- | :----: | -------------------------------- |
| bool | enable | True is enable. Default is False |

### Return

None

---

```c++
uint16_t getFontColor()
```

### Summary

Get the font color to be retained by the renderer.

### Arguments

None

### Return

| Type     | Description               |
| -------- | ------------------------- |
| uint16_t | Font color (16 bit color) |

---

```c++
uint16_t getBackgroundColor()
```

### Summary

Get the background color to be retained by the renderer.

### Arguments

None

### Return

| Type     | Description                     |
| -------- | ------------------------------- |
| uint16_t | Background color (16 bit color) |

---

```c++
void setFontSize(unsigned int pixel)
```

### Summary

Set the font size to be retained by the renderer.  
It is used when drawing with the `printf` function.

### Arguments

| Type         | Name  | Description   |
| ------------ | :---: | ------------- |
| unsigned int | pixel | New font size |

### Return

None

---

```c++
unsigned int getFontSize();
```

### Summary

Get the font size to be retained by the renderer.

### Arguments

None

### Return

| Type         | Description       |
| ------------ | ----------------- |
| unsigned int | Current font size |

---

```c++
void setLineSpaceRatio(double line_space_ratio)
```

### Summary

Gives the correction value for line spacing at line breaks.

### Arguments

| Type   |       Name       | Description                                             |
| ------ | :--------------: | ------------------------------------------------------- |
| double | line_space_ratio | The correction value for line spacing. Default is `1.0` |

### Return

None

---

```c++
double getLineSpaceRatio()
```

### Summary

Gives the correction value for line spacing at line breaks.

### Arguments

None

### Return

| Type   | Description                                |
| ------ | ------------------------------------------ |
| double | Current correction value for line spacing. |

---

```c++
void setCacheSize(unsigned int max_faces,
                  unsigned int max_sizes,
                  unsigned long max_bytes)
```

### Summary

Set cache size;

### Arguments

| Type          |   Name    | Description      |
| ------------- | :-------: | ---------------- |
| unsigned int  | max_faces | face cache size  |
| unsigned int  | max_sizes | sizes cache size |
| unsigned long | max_bytes | max cache size   |

### Return

None

---

## Other API

---

```c++
void showFreeTypeVersion()
```

### Summary

Outputs the version of the FreeType library in use.

### Arguments

None

### Return

None

### Note

If `ARDUINO_BOARD` is not defined, specify the output destination with the `OpenFontRender::setPrintFunc()` or `OpenFontRender::setSerial()` function in advance.

---

```c++
void getFreeTypeVersion(char *str)
```

### Summary

String the version of the FreeType library in use.

### Arguments

| Type    | Name | Description                        |
| ------- | :--: | ---------------------------------- |
| char \* | str  | (output) String for output version |

### Return

None

### Note

`OpenFontRender::FT_VERSION_STRING_SIZE` can be used to prepare an array large enough to store strings.

```c++
// in your main function

char ftv_str[OpenFontRender::FT_VERSION_STRING_SIZE] = {0};

...

render.getFreeTypeVersion(ftv_str);
```

---

```c++
void showCredit()
```

### Summary

Outputs a credit notation that complies with the FTL license.

### Arguments

None

### Return

None

### Note

If `ARDUINO_BOARD` is not defined, specify the output destination with the `OpenFontRender::setPrintFunc()` or `OpenFontRender::setSerial()` function in advance.

---

```c++
void getCredit(char *str)
```

### Summary

String a credit notation that complies with the FTL license.

### Arguments

| Type    | Name | Description                        |
| ------- | :--: | ---------------------------------- |
| char \* | str  | (output) String for output version |

### Return

None

### Note

`OpenFontRender::CREDIT_STRING_SIZE` can be used to prepare an array large enough to store strings.

```c++
// in your main function

char ft_credit[OpenFontRender::CREDIT_STRING_SIZE] = {0};

...

render.getCredit(ft_credit);
```

---

```c++
void setPrintFunc(Function f)
```

### Summary

Specifies the standard output destination for the system.

### Arguments

| Type     | Name | Description   |
| -------- | :--: | ------------- |
| Function |  f   | user function |

### Return

None

### Note

The function to be given must be a function like the one below that takes a specific argument.

```c++
void example_function (const char *s)
```

---

```c++
template <typename T> void setSerial(T &output)
```

### Summary

If you have an object that produces standard output and has a `print` method, you can use this function.

### Arguments

| Type |  Name  | Description |
| ---- | :----: | ----------- |
| Any  | output | user object |

### Return

None

### Note

Inside the function, the following process takes place.

```c++
template <typename T> void setSerial(T &drawer) {
    setPrintFunc(output.print);
}
```

---

```c++
void setDebugLevel(uint8_t level)
```

### Summary

Set the level for serial output.

### Arguments

| Type    | Name  | Description |
| ------- | :---: | ----------- |
| uint8_t | level | Debug Level |

### Return

None

### Note

The debug level is defined as follows, and multiple levels can be specified with the `OR` operator.  
Note that it will run slower, however!

| Level     | Description                                 |
| --------- | ------------------------------------------- |
| OFR_NONE  | Nothing is output.                          |
| OFR_ERROR | Output only when an error occurs.           |
| OFR_INFO  | Detailed notification of processing details |
| OFR_DEBUG | Output the rendered character               |

---
