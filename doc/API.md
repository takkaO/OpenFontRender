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
FT_Error drawChar(uint16_t unicode,
                  uint32_t x  = 0,
                  uint32_t y  = 0,
                  uint16_t fg = 0xFFFF,
                  uint16_t bg = 0x0000)
```

### Summary

Draws a single character specified in Unicode.

### Arguments

| Type     |  Name   | Description                            |
| -------- | :-----: | -------------------------------------- |
| uint16_t | unicode | Unicode number                         |
| uint32_t |    x    | Top-left X coordinate to start drawing |
| uint32_t |    y    | Top-left Y coordinate to start drawing |
| uint32_t |   fg    | Font color (16bit color)               |
| uint32_t |   bg    | Font background color (16bit color)    |

### Return

| Type     | Description                        |
| -------- | ---------------------------------- |
| FT_Error | FreeType error code. 0 is success. |

---

```c++
uint16_t drawString(const char *str,
                    uint32_t x  = 0,
                    uint32_t y  = 0,
                    uint16_t fg = 0xFFFF,
                    uint16_t bg = 0x0000)
```

### Summary

Draws the specified string.

### Arguments

| Type          | Name | Description                            |
| ------------- | :--: | -------------------------------------- |
| const char \* | str  | String to draw                         |
| uint32_t      |  x   | Top-left X coordinate to start drawing |
| uint32_t      |  y   | Top-left Y coordinate to start drawing |
| uint32_t      |  fg  | Font color (16bit color)               |
| uint32_t      |  bg  | Font background color (16bit color)    |

### Return

| Type     | Description                            |
| -------- | -------------------------------------- |
| uint16_t | Number of characters success to write. |

---

```c++
uint16_t drawString(const char *str,
                    uint32_t x,
                    uint32_t y,
                    uint16_t fg,
                    uint16_t bg,
                    FT_Error *error)
```

### Summary

Draws the specified string.

### Arguments

| Type          | Name  | Description                            |
| ------------- | :---: | -------------------------------------- |
| const char \* |  str  | String to draw                         |
| uint32_t      |   x   | Top-left X coordinate to start drawing |
| uint32_t      |   y   | Top-left Y coordinate to start drawing |
| uint32_t      |  fg   | Font color (16bit color)               |
| uint32_t      |  bg   | Font background color (16bit color)    |
| FT_Error\*    | error | FreeType error code. 0 is success.     |

### Return

| Type     | Description                            |
| -------- | -------------------------------------- |
| uint16_t | Number of characters success to write. |

---

```c++
uint16_t printf(const char *fmt, ...)
```

### Summary

Draws the specified format string.

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
void setRenderTaskMode(enum RenderMode mode)
```

### Summary

Set the operation mode of the renderer task.  
This is meaningless if you are not using FreeRTOS.
Default is `NORMAL`.

### Arguments

| Type            | Name | Description              |
| --------------- | :--: | ------------------------ |
| enum RenderMode | mode | `NORMAL` or `WITH_CACHE` |

### Return

None

---

## Cursor operation API

---

```c++
void setCursor(uint32_t x, uint32_t y)
```

### Summary

Set the position of the internal cursor held by the renderer.  
It is used when drawing with the `printf` function.

### Arguments

| Type     | Name | Description         |
| -------- | :--: | ------------------- |
| uint32_t |  x   | X coordinate to set |
| uint32_t |  y   | Y coordinate to set |

### Return

None

---

```c++
uint32_t getCursorX()
```

### Summary

Get the X coordinate of the internal cursor held by the renderer.

### Arguments

None

### Return

| Type     | Description          |
| -------- | -------------------- |
| uint32_t | Current X coordinate |

---

```c++
uint32_t getCursorY()
```

### Summary

Get the Y coordinate of the internal cursor held by the renderer.

### Arguments

None

### Return

| Type     | Description          |
| -------- | -------------------- |
| uint32_t | Current Y coordinate |

---

```c++
void seekCursor(int32_t delta_x, int32_t delta_y)
```

### Summary

Shifts the position of the internal cursor held by the renderer by the specified amount.  
This is useful for making fine adjustments.

### Arguments

| Type     |  Name   | Description          |
| -------- | :-----: | -------------------- |
| uint32_t | delta_x | X coordinate to move |
| uint32_t | delta_y | Y coordinate to move |

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
void setFontSize(size_t new_size)
```

### Summary

Set the font size to be retained by the renderer.  
It is used when drawing with the `printf` function.

### Arguments

| Type   |   Name   | Description   |
| ------ | :------: | ------------- |
| size_t | new_size | New font size |

### Return

None

---

```c++
size_t getFontSize();
```

### Summary

Get the font size to be retained by the renderer.

### Arguments

None

### Return

| Type   | Description       |
| ------ | ----------------- |
| size_t | Current font size |

---

## Other API

---

```c++
void showFreeTypeVersion(Print &output = Serial)
```

### Summary

Outputs the version of the FreeType library in use.

### Arguments

| Type  |  Name  | Description  |
| ----- | :----: | ------------ |
| Print | output | Print object |

### Return

None

---

```c++
void showCredit(Print &output = Serial)
```

### Summary

Outputs a credit notation that complies with the FTL license.

### Arguments

| Type  |  Name  | Description  |
| ----- | :----: | ------------ |
| Print | output | Print object |

### Return

None

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
