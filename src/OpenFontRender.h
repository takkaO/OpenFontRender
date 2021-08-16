#ifndef OPEN_FONT_RENDER_H
#define OPEN_FONT_RENDER_H

#include <Arduino.h>
#include <stdarg.h>
#include <stdio.h>

#include "ft2build.h"
#include FT_CACHE_H
#include FT_FREETYPE_H

#undef min
#include <functional>
#define setDrawPixel(F) set_drawPixel([&](int32_t x, int32_t y, int16_t c) { return F(x, y, c); })
#define setStartWrite(F) set_startWrite([&](void) { return F(); })
#define setEndWrite(F) set_endWrite([&](void) { return F(); })

#include "FileSupport.h"

enum OFR_DEBUG_LEVEL {
	OFR_NONE  = 0,
	OFR_ERROR = 1,
	OFR_INFO  = 2,
	OFR_DEBUG = 4,
	OFR_RAW   = 8,
};

enum RenderMode {
	NORMAL,
	WITH_CACHE
};

class OpenFontRender {
public:
	OpenFontRender();
	void setUseRenderTask(bool enable);
	void setRenderTaskMode(enum RenderMode mode);
	void setCursor(uint32_t x, uint32_t y);
	uint32_t getCursorX();
	uint32_t getCursorY();
	void seekCursor(int32_t delta_x, int32_t delta_y);
	void setFontColor(uint16_t font_color);
	void setFontColor(uint16_t font_color, uint16_t font_bgcolor);
	void setFontSize(size_t new_size);

	FT_Error loadFont(const unsigned char *data, size_t size);
	FT_Error loadFont(const char *fpath);
	FT_Error drawChar(uint16_t unicode,
	                  uint32_t x  = 0,
	                  uint32_t y  = 0,
	                  uint16_t fg = 0xFFFF,
	                  uint16_t bg = 0x0000);
	uint16_t drawString(const char *str,
	                    uint32_t x  = 0,
	                    uint32_t y  = 0,
	                    uint16_t fg = 0xFFFF,
	                    uint16_t bg = 0x0000);
	uint16_t drawString(const char *str,
	                    uint32_t x,
	                    uint32_t y,
	                    uint16_t fg,
	                    uint16_t bg,
	                    FT_Error *error);

	uint16_t printf(const char *fmt, ...);

	void showFreeTypeVersion(Print &output = Serial);
	void showCredit(Print &output = Serial);
	void setDebugLevel(uint8_t level);

	template <typename T>
	void setDrawer(T &drawer) {
		set_drawPixel([&](int32_t x, int32_t y, int16_t c) { return drawer.drawPixel(x, y, c); });
		set_startWrite([&](void) { return drawer.startWrite(); });
		set_endWrite([&](void) { return drawer.endWrite(); });
	}

	// Direct calls are deprecated.
	void set_drawPixel(std::function<void(int32_t, int32_t, uint16_t)> user_func);
	void set_startWrite(std::function<void(void)> user_func);
	void set_endWrite(std::function<void(void)> user_func);

private:
	void draw2screen(FTC_SBit sbit, uint32_t x, uint32_t y, uint16_t fg, uint16_t bg);
	uint16_t decodeUTF8(uint8_t *buf, uint16_t *index, uint16_t remaining);
	uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
	uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc);

	std::function<void(int32_t, int32_t, uint16_t)> _drawPixel;
	std::function<void(void)> _startWrite;
	std::function<void(void)> _endWrite;

	FTC_Manager _ftc_manager;
	FTC_CMapCache _ftc_cmap_cache;
	FTC_SBitCache _ftc_sbit_cache;
	uint8_t _face_id;

	enum RenderMode _render_mode;
	struct FontParameter {
		size_t size;
		uint16_t fg_color;
		uint16_t bg_color;
	};
	struct FontParameter _font;

	struct Cursor {
		uint32_t x;
		uint32_t y;
	};
	struct Cursor _cursor;

	uint8_t _debug_level;
};

#endif