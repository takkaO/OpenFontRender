// -------------------------------------------------------
//  OpenFontRender.h
//
//  Copyright (c) 2021 takkaO
//
//  If you use, modify or redistribute this file as part of
//  the original repository, please follow the repository's license.
//
// -------------------------------------------------------

#ifndef OPEN_FONT_RENDER_H
#define OPEN_FONT_RENDER_H

#if defined(ARDUINO_BOARD)
	#include <Arduino.h>
#endif
#include <stdarg.h>
#include <stdio.h>

#include "ft2build.h"
#include FT_CACHE_H
#include FT_FREETYPE_H

#include <functional>
#include <queue>
#include <string>
#include <vector>

#include "FileSupport.h"

/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/*/
//
//  Constant definition
//
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/*/

enum OFR_DEBUG_LEVEL {
	OFR_NONE  = 0,
	OFR_ERROR = 1,
	OFR_INFO  = 2,
	OFR_DEBUG = 4,
	OFR_RAW   = 8,
};

enum class Align {
	Left,
	Center,
	Right
};

enum class Layout {
	Horizontal,
	Vertical
};

enum class Drawing {
	Execute,
	Skip
};

namespace OFR {
	/* USER DO NOT USE DIRECTORY IN THIS SCOPE ELEMENTS */
	enum LoadFontFrom {
		FROM_FILE,
		FROM_MEMORY
	};

	typedef struct FaceRec_ {
		char *filepath;      // ttf file path
		unsigned char *data; // ttf array
		size_t data_size;    // ttf array size
		uint8_t face_index;  // face index (default is 0)
	} FaceRec, *Face;
};

/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/*/
//
//  Class definition
//
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/*/
#define setDrawPixel(F) set_drawPixel([&](int32_t x, int32_t y, uint16_t c) { return F(x, y, c); })
#define setStartWrite(F) set_startWrite([&](void) { return F(); })
#define setEndWrite(F) set_endWrite([&](void) { return F(); })
#define setPrintFunc(F) set_printFunc([&](const char *s) { return F(s); })

class OpenFontRender {
public:
	static const unsigned char MAIN_VERSION  = 1;
	static const unsigned char MINER_VERSION = 0;

	static const unsigned char CACHE_SIZE_NO_LIMIT    = 0;
	static const unsigned char FT_VERSION_STRING_SIZE = 32;
	static const unsigned char CREDIT_STRING_SIZE     = 128;

	OpenFontRender();
	void setUseRenderTask(bool enable);
	void setRenderTaskStackSize(unsigned int stack_size);
	void setCursor(int32_t x, int32_t y);
	int32_t getCursorX();
	int32_t getCursorY();
	void seekCursor(int32_t delta_x, int32_t delta_y);
	void setFontColor(uint16_t font_color);
	void setFontColor(uint16_t font_color, uint16_t font_bgcolor);
	void setFontColor(uint8_t r, uint8_t g, uint8_t b);
	void setFontColor(uint8_t fr,
	                  uint8_t fg,
	                  uint8_t fb,
	                  uint8_t br,
	                  uint8_t bg,
	                  uint8_t bb);
	void setBackgroundColor(uint16_t font_bgcolor);
	void setTransparentBackground(bool enable);
	uint16_t getFontColor();
	uint16_t getBackgroundColor();
	void setFontSize(unsigned int pixel);
	unsigned int getFontSize();
	double setLineSpaceRatio(double line_space_ratio);
	double getLineSpaceRatio();
	void setCacheSize(unsigned int max_faces, unsigned int max_sizes, unsigned long max_bytes);

	FT_Error loadFont(const unsigned char *data, size_t size, uint8_t target_face_index = 0);
	FT_Error loadFont(const char *fpath, uint8_t target_face_index = 0);
	void unloadFont();

	uint16_t drawHString(const char *str,
	                     int32_t x,
	                     int32_t y,
	                     uint16_t fg,
	                     uint16_t bg,
	                     Align align,
	                     Drawing drawing,
	                     FT_BBox &abbox,
	                     FT_Error &error);
	FT_Error drawChar(char character,
	                  int32_t x   = 0,
	                  int32_t y   = 0,
	                  uint16_t fg = 0xFFFF,
	                  uint16_t bg = 0x0000,
	                  Align align = Align::Left);
	uint16_t drawString(const char *str,
	                    int32_t x     = 0,
	                    int32_t y     = 0,
	                    uint16_t fg   = 0xFFFF,
	                    uint16_t bg   = 0x0000,
	                    Layout layout = Layout::Horizontal);
	uint16_t cdrawString(const char *str,
	                     int32_t x     = 0,
	                     int32_t y     = 0,
	                     uint16_t fg   = 0xFFFF,
	                     uint16_t bg   = 0x0000,
	                     Layout layout = Layout::Horizontal);
	uint16_t rdrawString(const char *str,
	                     int32_t x     = 0,
	                     int32_t y     = 0,
	                     uint16_t fg   = 0xFFFF,
	                     uint16_t bg   = 0x0000,
	                     Layout layout = Layout::Horizontal);

	uint16_t printf(const char *fmt, ...);
	uint16_t cprintf(const char *fmt, ...);
	uint16_t rprintf(const char *fmt, ...);

	FT_BBox calculateBoundingBoxFmt(int32_t x, int32_t y, unsigned int font_size, Align align, Layout layout, const char *fmt, ...);
	FT_BBox calculateBoundingBox(int32_t x, int32_t y, unsigned int font_size, Align align, Layout layout, const char *str);
	unsigned int calculateFitFontSizeFmt(uint32_t limit_width, uint32_t limit_height, Layout layout, const char *fmt, ...);
	unsigned int calculateFitFontSize(uint32_t limit_width, uint32_t limit_height, Layout layout, const char *str);

	void showFreeTypeVersion();
	void showCredit();
	void getFreeTypeVersion(char *str);
	void getCredit(char *str);
	void setDebugLevel(uint8_t level);

	template <typename T>
	void setDrawer(T &drawer) {
		set_drawPixel([&](int32_t x, int32_t y, uint16_t c) { return drawer.drawPixel(x, y, c); });
		set_drawFastHLine([&](int32_t x, int32_t y, int32_t w, uint16_t c) { return drawer.drawFastHLine(x, y, w, c); });
		set_startWrite([&](void) { return drawer.startWrite(); });
		set_endWrite([&](void) { return drawer.endWrite(); });
	}

	// Direct calls are deprecated.
	void set_drawPixel(std::function<void(int32_t, int32_t, uint16_t)> user_func);
	void set_drawFastHLine(std::function<void(int32_t, int32_t, int32_t, uint16_t)> user_func);
	void set_startWrite(std::function<void(void)> user_func);
	void set_endWrite(std::function<void(void)> user_func);

	/* Static member method */
	template <typename T>
	static void setSerial(T &output) {
		set_printFunc([&](const char *s) { return output.print(s); });
	}
	// Direct calls are deprecated.
	static void set_printFunc(std::function<void(const char *)> user_func);

	struct Cursor {
		int32_t x;
		int32_t y;
	};

private:
	FT_Error loadFont(enum OFR::LoadFontFrom from);
	uint32_t getFontMaxHeight();
	void draw2screen(FT_BitmapGlyph glyph, uint32_t x, uint32_t y, uint16_t fg, uint16_t bg);
	uint16_t decodeUTF8(uint8_t *buf, uint16_t *index, uint16_t remaining);
	uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
	uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc);

	std::function<void(int32_t, int32_t, uint16_t)> _drawPixel;
	std::function<void(int32_t, int32_t, int32_t, uint16_t)> _drawFastHLine;
	std::function<void(void)> _startWrite;
	std::function<void(void)> _endWrite;

	FTC_Manager _ftc_manager;
	FTC_CMapCache _ftc_cmap_cache;
	FTC_ImageCache _ftc_image_cache;

	unsigned int _max_faces;
	unsigned int _max_sizes;
	unsigned long _max_bytes;
	bool _transparent_background;

	OFR::FaceRec _face_id;
	struct Cursor _cursor;

	struct FontParameter {
		double line_space_ratio;
		unsigned int size;
		uint16_t fg_color;
		uint16_t bg_color;
		bool support_vertical;
	};
	struct FontParameter _font;

	uint8_t _debug_level;
};

#endif