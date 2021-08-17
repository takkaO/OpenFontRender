# API

```c++
FT_Error loadFont(const unsigned char *data, size_t size)
```

### Features

コードに埋め込まれたフォントデータを読み込みます。

### Args

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

### Features

外部ストレージ（SD カード）からフォントデータを読み込みます。

### Args

| Type          | Name  | Description |
| ------------- | :---: | ----------- |
| const char \* | fpath | File path   |

### Return

| Type     | Description                        |
| -------- | ---------------------------------- |
| FT_Error | FreeType error code. 0 is success. |

---

```c++
void unloadFont()
```

### Features

外部ストレージ（SD カード）からフォントデータを読み込みます。

### Args

None

### Return

None

---

```c++
FT_Error drawChar(uint16_t unicode,
                  uint32_t x  = 0,
                  uint32_t y  = 0,
                  uint16_t fg = 0xFFFF,
                  uint16_t bg = 0x0000)
```

### Features

指定された１文字を描画します。

### Args

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

### Features

指定された文字列を描画します。

### Args

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

### Features

指定された文字列を描画します。

### Args

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

### Features

指定されたフォーマット文字列を描画します。

### Args

| Type          | Name | Description   |
| ------------- | :--: | ------------- |
| const char \* | fmt  | Format string |

### Return

| Type     | Description                            |
| -------- | -------------------------------------- |
| uint16_t | Number of characters success to write. |

---

```c++
void showFreeTypeVersion(Print &output = Serial)
```

### Features

使用中の FreeType ライブラリのバージョンを出力します。

### Args

| Type  |  Name  | Description  |
| ----- | :----: | ------------ |
| Print | output | Print object |

### Return

None

---

```c++
void showCredit(Print &output = Serial)
```

### Features

FTL ライセンスに準拠したクレジット表記を出力します。

### Args

| Type  |  Name  | Description  |
| ----- | :----: | ------------ |
| Print | output | Print object |

### Return

None

---

```c++
void setDebugLevel(uint8_t level)
```

### Features

シリアル出力するレベルを指定します。

### Args

| Type       | Name  | Description |
| ---------- | :---: | ----------- |
| 　 uint8_t | level | Debug Level |

### Return

None

### Note

デバッグレベルは以下のように定義されており、`OR`演算子で複数指定することができます。

| Level     | Description                      |
| --------- | -------------------------------- |
| OFR_NONE  | 何も出力しません                 |
| OFR_ERROR | エラーが起きた場合のみ出力します |
| OFR_INFO  | 処理内容を細かく通知します       |
| OFR_DEBUG | レンダリングした文字を出力します |

---

```c++
void setUseRenderTask(bool enable)
```

### Features

レンダータスクを作成するかどうかを指定します。
FreeRTOS を使用していない場合は、意味がありません。

### Args

| Type |  Name  | Description                      |
| ---- | :----: | -------------------------------- |
| bool | enable | Enable (true) or Disable (false) |

### Return

None

### Note

If you NOT use FreeType 2.4.12, recommend to set Enable.

---

```c++
void setRenderTaskMode(enum RenderMode mode)
```

### Features

レンダータスクの動作モードを指定します。
FreeRTOS を使用していない場合は、呼び出しても何もしません。

### Args

| Type            | Name | Description              |
| --------------- | :--: | ------------------------ |
| enum RenderMode | mode | `NORMAL` or `WITH_CACHE` |

### Return

None

---

```c++
void setCursor(uint32_t x, uint32_t y)
```

### Features

レンダラーが保持する内部カーソルの位置を指定します。
`printf`関数で描画する際に使用されます。

### Args

| Type     | Name | Description         |
| -------- | :--: | ------------------- |
| uint32_t |  x   | X Coordinate to set |
| uint32_t |  y   | Y Coordinate to set |

### Return

None
