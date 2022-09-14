// -------------------------------------------------------
//  OpenFontRender.cpp
//
//  Copyright (c) 2021 takkaO
//
//  If you use, modify or redistribute this file as part of
//  the original repository, please follow the repository's license.
//
// -------------------------------------------------------

#include "OpenFontRender.h"

/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/*/
//
//  Data Structure Definition
//  (Use only within this file.)
//
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/*/

enum LoadFontFrom {
	FROM_FILE,
	FROM_MEMORY
};

typedef struct {
	enum LoadFontFrom from; // data source
	char *filepath;         // ttf file path
	unsigned char *data;    // ttf array
	size_t data_size;       // ttf array size
	uint8_t debug_level;    // debug level
} FontDataInfo;

typedef struct {
	FT_Glyph glyph;
	FT_Vector pos;
} RenderStringInfo;

#ifdef FREERTOS_CONFIG_H
enum RenderTaskStatus {
	IDLE,
	LOCK,
	RENDERING,
	END_ERROR,
	END_SUCCESS
};

typedef struct {
	FT_Glyph image;      // result
	FT_Error error;      // ft_error
	uint8_t debug_level; // debug level
} RenderTaskParameter;
#endif

/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/*/
//
//  Function Definition
//  (See below more ditails.)
//
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/*/

FT_Error ftc_face_requester(FTC_FaceID face_id,
                            FT_Library library,
                            FT_Pointer request_data,
                            FT_Face *face);
void debugPrintf(uint8_t level, const char *fmt, ...);

#ifdef FREERTOS_CONFIG_H
void RenderTask(void *pvParameters);
#endif

/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/*/
//
//  Global Variables
//  (Use only within this file.)
//
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/*/

FT_Library g_FtLibrary;
uint8_t g_AvailableFaceId = 0;
bool g_NeedInitialize     = true;

#ifdef FREERTOS_CONFIG_H
TaskHandle_t g_RenderTaskHandle                   = NULL;
volatile enum RenderTaskStatus g_RenderTaskStatus = IDLE;
volatile bool g_UseRenderTask                     = (FREETYPE_MAJOR == 2 && FREETYPE_MINOR == 4 && FREETYPE_PATCH == 12) ? false : true;
unsigned int g_RenderTaskStackSize                = 8192 + 8192 + 4096;
RenderTaskParameter g_TaskParameter;
#endif

std::function<void(const char *)> g_Print;

/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/*/
//
//  OpenFontRender Class Public Methods
//
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/*/

OpenFontRender::OpenFontRender() {
	g_Print = [](const char *s) {
#ifdef ARDUINO_BOARD
		Serial.print(s);
#else
		return;
#endif
	};

	// TODO: Automatic support some micro computers
	_drawPixel = [](int x, int y, int c) {
		static bool flag = true;
		if (flag) {
			g_Print("\n** [Warning] Please set drawPixel() using setDrawPixel(). **\n");
			flag = false;
		}
		return;
	};
	_startWrite = []() { return; };
	_endWrite   = []() { return; };

	_max_faces = OFR_CACHE_SIZE_NO_LIMIT;
	_max_sizes = OFR_CACHE_SIZE_NO_LIMIT;
	_max_bytes = OFR_CACHE_SIZE_NO_LIMIT;

	_font.line_space_ratio = 1.0;    // Set default line space ratio
	_font.size             = 44;     // Set default font size
	_font.fg_color         = 0xFFFF; // Set default font color (White)
	_font.bg_color         = 0x0000; // Set default background color (Black)
	_font.support_vertical = false;
	_debug_level           = OFR_NONE;

	_transparent_background = false;
}

void OpenFontRender::setUseRenderTask(bool enable) {
#ifdef FREERTOS_CONFIG_H
	g_UseRenderTask = enable;
#endif
}

void OpenFontRender::setRenderTaskStackSize(unsigned int stack_size) {
#ifdef FREERTOS_CONFIG_H
	g_RenderTaskStackSize = stack_size;
#endif
}

void OpenFontRender::setCursor(int32_t x, int32_t y) {
	_cursor = {x, y};
}

int32_t OpenFontRender::getCursorX() {
	return _cursor.x;
}

int32_t OpenFontRender::getCursorY() {
	return _cursor.y;
}

void OpenFontRender::seekCursor(int32_t delta_x, int32_t delta_y) {
	_cursor.x += delta_x;
	_cursor.y += delta_y;
}

void OpenFontRender::setFontColor(uint16_t font_color) {
	_font.fg_color = font_color;
}

void OpenFontRender::setFontColor(uint16_t font_color, uint16_t font_bgcolor) {
	_font.fg_color = font_color;
	_font.bg_color = font_bgcolor;
}

void OpenFontRender::setFontColor(uint8_t r, uint8_t g, uint8_t b) {
	_font.fg_color = color565(r, g, b);
}

void OpenFontRender::setFontColor(uint8_t fr, uint8_t fg, uint8_t fb, uint8_t br, uint8_t bg, uint8_t bb) {
	_font.fg_color = color565(fr, fg, fb);
	_font.bg_color = color565(br, bg, bb);
}

void OpenFontRender::setBackgroundColor(uint16_t font_bgcolor) {
	_font.bg_color = font_bgcolor;
}

uint16_t OpenFontRender::getFontColor() {
	return _font.fg_color;
}

uint16_t OpenFontRender::getBackgroundColor() {
	return _font.bg_color;
}

void OpenFontRender::setTransparentBackground(bool enable) {
	_transparent_background = enable;
}

void OpenFontRender::setFontSize(unsigned int pixel) {
	_font.size = pixel;
}

unsigned int OpenFontRender::getFontSize() {
	return _font.size;
}

double OpenFontRender::setLineSpaceRatio(double line_space_ratio) {
	if (line_space_ratio <= 0) {
		// Invalid argument
		return _font.line_space_ratio;
	}
	_font.line_space_ratio = line_space_ratio;
	return _font.line_space_ratio;
}

double OpenFontRender::getLineSpaceRatio() {
	return _font.line_space_ratio;
}

void OpenFontRender::setCacheSize(unsigned int max_faces, unsigned int max_sizes, unsigned long max_bytes) {
	_max_faces = max_faces;
	_max_sizes = max_sizes;
	_max_bytes = max_bytes;
}

FT_Error OpenFontRender::loadFont(const unsigned char *data, size_t size) {
	FT_Face face;
	FT_Error error;
	FontDataInfo info = {FROM_MEMORY, nullptr, (unsigned char *)data, size, _debug_level};

	if (g_NeedInitialize) {
		error = FT_Init_FreeType(&g_FtLibrary);
		if (error) {
			debugPrintf((_debug_level & OFR_ERROR), "FT_Init_FreeType error: 0x%02X\n", error);
			return error;
		}
		g_NeedInitialize = false;
	}

	_face_id = g_AvailableFaceId++;
	// 現在の引数は適当
	error = FTC_Manager_New(g_FtLibrary, _max_faces, _max_sizes, _max_bytes, &ftc_face_requester, &info, &_ftc_manager);
	if (error) {
		debugPrintf((_debug_level & OFR_ERROR), "FTC_Manager_New error: 0x%02X\n", error);
		return error;
	}

	error = FTC_Manager_LookupFace(_ftc_manager, (FTC_FaceID)_face_id, &face);
	if (error) {
		debugPrintf((_debug_level & OFR_ERROR), "FTC_Manager_LookupFace error: 0x%02X\n", error);
		return error;
	}

	error = FTC_CMapCache_New(_ftc_manager, &_ftc_cmap_cache);
	if (error) {
		debugPrintf((_debug_level & OFR_ERROR), "FTC_CMapCache_New error: 0x%02X\n", error);
		return error;
	}

	error = FTC_ImageCache_New(_ftc_manager, &_ftc_image_cache);
	if (error) {
		debugPrintf((_debug_level & OFR_ERROR), "FTC_ImageCache_New error: 0x%02X\n", error);
		return error;
	}

	if (FT_HAS_VERTICAL(face) == 0) {
		// Current font does NOT support vertical layout
		_font.support_vertical = false;
	} else {
		_font.support_vertical = true;
	}

	return FT_Err_Ok;
}

FT_Error OpenFontRender::loadFont(const char *fpath) {
	FT_Face face;
	FT_Error error;
	FontDataInfo info = {FROM_FILE, (char *)fpath, nullptr, 0, _debug_level};

	if (g_NeedInitialize) {
		error = FT_Init_FreeType(&g_FtLibrary);
		if (error) {
			debugPrintf((_debug_level & OFR_ERROR), "FT_Init_FreeType error: 0x%02X\n", error);
			return error;
		}
		g_NeedInitialize = false;
	}

	_face_id = g_AvailableFaceId++;
	// 現在の引数は適当
	error = FTC_Manager_New(g_FtLibrary, _max_faces, _max_sizes, _max_bytes, &ftc_face_requester, &info, &_ftc_manager);
	if (error) {
		debugPrintf((_debug_level & OFR_ERROR), "FTC_Manager_New error: 0x%02X\n", error);
		return error;
	}

	error = FTC_Manager_LookupFace(_ftc_manager, (FTC_FaceID)_face_id, &face);
	if (error) {
		debugPrintf((_debug_level & OFR_ERROR), "FTC_Manager_LookupFace error: 0x%02X\n", error);
		return error;
	}

	error = FTC_CMapCache_New(_ftc_manager, &_ftc_cmap_cache);
	if (error) {
		debugPrintf((_debug_level & OFR_ERROR), "FTC_CMapCache_New error: 0x%02X\n", error);
		return error;
	}

	error = FTC_ImageCache_New(_ftc_manager, &_ftc_image_cache);
	if (error) {
		debugPrintf((_debug_level & OFR_ERROR), "FTC_ImageCache_New error: 0x%02X\n", error);
		return error;
	}

	if (FT_HAS_VERTICAL(face) == 0) {
		// Current font does NOT support vertical layout
		_font.support_vertical = false;
	} else {
		_font.support_vertical = true;
	}

	return FT_Err_Ok;
}

void OpenFontRender::unloadFont() {
	FTC_Manager_Reset(_ftc_manager);
	FTC_Manager_Done(_ftc_manager);
	FT_Done_FreeType(g_FtLibrary);
	g_NeedInitialize = true;
}

uint16_t OpenFontRender::drawHString(const char *str,
                                     int32_t x,
                                     int32_t y,
                                     uint16_t fg,
                                     uint16_t bg,
                                     Align align,
                                     Drawing drawing,
                                     FT_BBox &abbox,
                                     FT_Error &error) {

	FT_Face face;
	FT_Glyph aglyph;
	uint16_t written_char_num    = 0;
	bool detect_control_char     = false;
	Cursor initial_position      = {x, y};
	Cursor current_line_position = {x, y};
	FT_Vector offset             = {0, 0};
	FT_Size asize                = NULL;

	FT_BBox bbox;
	bbox.xMin = bbox.yMin = LONG_MAX;
	bbox.xMax = bbox.yMax = LONG_MIN;

	abbox.xMin = abbox.yMin = LONG_MAX;
	abbox.xMax = abbox.yMax = LONG_MIN;

	FTC_ImageTypeRec image_type;
	image_type.face_id = (FTC_FaceID)_face_id;
	image_type.width   = 0;
	image_type.height  = _font.size;
	image_type.flags   = FT_LOAD_DEFAULT;

	FTC_ScalerRec scaler;
	scaler.face_id = (FTC_FaceID)_face_id;
	scaler.width   = 0;
	scaler.height  = _font.size;
	scaler.pixel   = true;
	scaler.x_res   = 0;
	scaler.y_res   = 0;

	std::vector<RenderStringInfo> v_rsi;
	v_rsi.reserve(256); // Allocate memory for 256 characters for efficiency

	// decode UTF8
	uint16_t unicode;
	uint16_t len = strlen(str);
	uint16_t n   = 0;
	std::queue<FT_UInt32> unicode_q;
	while (n < len) {
		uint16_t unicode = decodeUTF8((uint8_t *)str, &n, len - n);
		unicode_q.push(unicode);
	}

	error = FTC_Manager_LookupSize(_ftc_manager, &scaler, &asize);
	if (error) {
		return written_char_num;
	}
	face              = asize->face;
	FT_Int cmap_index = FT_Get_Charmap_Index(face->charmap);

	// Rendering loop
	while (unicode_q.size() != 0) {
		RenderStringInfo rsi;
		detect_control_char   = false;
		current_line_position = {x, y};
		bbox.xMin = bbox.yMin = LONG_MAX;
		bbox.xMax = bbox.yMax = LONG_MIN;
		v_rsi.clear();

		// Glyph extraction
		while (unicode_q.size() != 0 && detect_control_char == false) {
			unicode = unicode_q.front();
			switch (unicode) {
			case '\r':
				[[fallthrough]]; // Fall Through
			case '\n':
				detect_control_char = true;
				break;
			default:
				rsi.pos             = {x, y};
				FT_UInt glyph_index = FTC_CMapCache_Lookup(_ftc_cmap_cache,
				                                           (FTC_FaceID)_face_id,
				                                           cmap_index,
				                                           unicode);

				error = FTC_ImageCache_Lookup(_ftc_image_cache, &image_type, glyph_index, &aglyph, NULL);
				if (error) {
					debugPrintf((_debug_level & OFR_ERROR), "FTC_ImageCache_Lookup error: 0x%02X\n", error);
					return written_char_num;
				}

				error = FT_Glyph_Copy(aglyph, &rsi.glyph);
				if (error) {
					debugPrintf((_debug_level & OFR_ERROR), "FT_Glyph_Copy error: 0x%02X\n", error);
					return written_char_num;
				}
				v_rsi.push_back(rsi);

				x += (aglyph->advance.x >> 16);
			}
			unicode_q.pop();
		}

		// Caluculate BBox
		for (int i = 0; i < v_rsi.size(); i++) {
			FT_BBox glyph_bbox;
			FT_Glyph_Get_CBox(v_rsi[i].glyph, FT_GLYPH_BBOX_PIXELS, &glyph_bbox);

			// Move coordinates on the grid
			glyph_bbox.xMin += v_rsi[i].pos.x;
			glyph_bbox.xMax += v_rsi[i].pos.x;
			glyph_bbox.yMin += v_rsi[i].pos.y;
			glyph_bbox.yMax += v_rsi[i].pos.y;

			// Merge bbox
			bbox.xMin = std::min(bbox.xMin, glyph_bbox.xMin);
			bbox.yMin = std::min(bbox.yMin, glyph_bbox.yMin);
			bbox.xMax = std::max(bbox.xMax, glyph_bbox.xMax);
			bbox.yMax = std::max(bbox.yMax, glyph_bbox.yMax);
		}

		/* Check that we really grew the string bbox */
		if (bbox.xMin > bbox.xMax) {
			// Failed
			bbox.xMin = 0;
			bbox.yMin = 0;
			bbox.xMax = 0;
			bbox.yMax = 0;
		} else {
			// Transform coordinate space differences
			bbox.yMax = y - (bbox.yMax - y) + ((face->size->metrics.ascender) >> 6);
			bbox.yMin = y + (y - bbox.yMin) + ((face->size->metrics.ascender) >> 6);
			if (bbox.yMax < bbox.yMin) {
				std::swap(bbox.yMax, bbox.yMin);
			}
			// Correct slight misalignment of each axis
			offset.x = bbox.xMin - current_line_position.x;
			offset.y = bbox.yMin - current_line_position.y;
		}

		switch (align) {
		case Align::Left:
			// Nothing to do
			break;
		case Align::Center:
			offset.x += ((bbox.xMax - bbox.xMin) / 2);
			break;
		case Align::Right:
			offset.x = (bbox.xMax - bbox.xMin);
			break;
		default:
			break;
		}

		bbox.xMin -= offset.x;
		bbox.xMax -= offset.x;
		bbox.yMin -= offset.y;
		bbox.yMax -= offset.y;

		if (drawing == Drawing::Execute) {
			for (int i = 0; i < v_rsi.size(); i++) {
				FT_Glyph image = v_rsi[i].glyph;
				FT_Vector pos  = {(v_rsi[i].pos.x - offset.x), (v_rsi[i].pos.y - offset.y)};
				// Change baseline to left top
				pos.y += ((face->size->metrics.ascender) >> 6);

#ifdef FREERTOS_CONFIG_H
				if (g_UseRenderTask) {
					if (g_RenderTaskHandle == NULL) {
						debugPrintf((_debug_level & OFR_INFO), "Create render task\n");
						const uint8_t RUNNING_CORE = 1;
						const uint8_t PRIORITY     = 1;
						xTaskCreateUniversal(RenderTask,
						                     "RenderTask",
						                     g_RenderTaskStackSize, // Seems to need a lot of memory.
						                     NULL,
						                     PRIORITY,
						                     &g_RenderTaskHandle,
						                     RUNNING_CORE);
					}
					while (g_RenderTaskStatus != IDLE) {
						vTaskDelay(1);
					}
					g_RenderTaskStatus          = LOCK;
					g_TaskParameter.image       = image;
					g_TaskParameter.debug_level = _debug_level;

					g_RenderTaskStatus = RENDERING;
					while (g_RenderTaskStatus == RENDERING) {
						vTaskDelay(1);
					}
					debugPrintf((g_TaskParameter.debug_level & OFR_INFO), "Render task Finish\n");
					image              = g_TaskParameter.image;
					error              = g_TaskParameter.error;
					g_RenderTaskStatus = IDLE;
				} else {
					error = FT_Glyph_To_Bitmap(&image, FT_RENDER_MODE_NORMAL, NULL, false);
				}
#else
				error = FT_Glyph_To_Bitmap(&image, FT_RENDER_MODE_NORMAL, NULL, false);
#endif

				if (!error) {
					FT_BitmapGlyph bit = (FT_BitmapGlyph)image;

					draw2screen(bit, pos.x, pos.y, fg, bg);

					FT_Done_Glyph(image);
					written_char_num++;
				}
			}
		}

		if (detect_control_char) {
			switch (unicode) {
			case '\r':
				x = initial_position.x;
				break;
			case '\n':
				x = initial_position.x;
				y += getFontMaxHeight() * _font.line_space_ratio;
				break;
			default:
				// No supported control char
				break;
			}
		}

		// Merge bbox
		abbox.xMin = std::min(bbox.xMin, abbox.xMin);
		abbox.yMin = std::min(bbox.yMin, abbox.yMin);
		abbox.xMax = std::max(bbox.xMax, abbox.xMax);
		abbox.yMax = std::max(bbox.yMax, abbox.yMax);
	} /* End of rendering loop */

	if (detect_control_char && unicode == '\n') {
		// If string end with '\n' control char, expand bbox
		abbox.yMax += getFontMaxHeight() * _font.line_space_ratio;
	}

	_cursor = {x, y};
	return written_char_num;
}

FT_Error OpenFontRender::drawChar(char character, int32_t x, int32_t y, uint16_t fg, uint16_t bg, Align align) {
	FT_Error error;
	FT_BBox bbox;

	drawHString(&character, x, y, fg, bg, Align::Left, Drawing::Execute, bbox, error);

	return FT_Err_Ok;
}

uint16_t OpenFontRender::drawString(const char *str, int32_t x, int32_t y, uint16_t fg, uint16_t bg, Layout layout) {
	FT_Error error;
	FT_BBox bbox;

	switch (layout) {
	case Layout::Horizontal:
		return drawHString(str, x, y, fg, bg, Align::Left, Drawing::Execute, bbox, error);
	case Layout::Vertical:
		// Not support now
		return 0;
	default:
		return 0;
	}
}

uint16_t OpenFontRender::cdrawString(const char *str, int32_t x, int32_t y, uint16_t fg, uint16_t bg, Layout layout) {
	FT_Error error;
	FT_BBox bbox;

	switch (layout) {
	case Layout::Horizontal:
		return drawHString(str, x, y, fg, bg, Align::Center, Drawing::Execute, bbox, error);
	case Layout::Vertical:
		// Not support now
		return 0;
	default:
		return 0;
	}
}

uint16_t OpenFontRender::rdrawString(const char *str, int32_t x, int32_t y, uint16_t fg, uint16_t bg, Layout layout) {
	FT_Error error;
	FT_BBox bbox;

	switch (layout) {
	case Layout::Horizontal:
		return drawHString(str, x, y, fg, bg, Align::Right, Drawing::Execute, bbox, error);
	case Layout::Vertical:
		// Not support now
		return 0;
	default:
		return 0;
	}
}

uint16_t OpenFontRender::printf(const char *fmt, ...) {
	char str[256] = {0};
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(str, 256, fmt, ap);
	va_end(ap);

	return drawString(str, _cursor.x, _cursor.y, _font.fg_color, _font.bg_color, Layout::Horizontal);
}

uint16_t OpenFontRender::cprintf(const char *fmt, ...) {
	char str[256] = {0};
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(str, 256, fmt, ap);
	va_end(ap);

	return cdrawString(str, _cursor.x, _cursor.y, _font.fg_color, _font.bg_color, Layout::Horizontal);
}

uint16_t OpenFontRender::rprintf(const char *fmt, ...) {
	char str[256] = {0};
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(str, 256, fmt, ap);
	va_end(ap);

	return rdrawString(str, _cursor.x, _cursor.y, _font.fg_color, _font.bg_color, Layout::Horizontal);
}

FT_BBox OpenFontRender::calculateBoundingBoxFmt(int32_t x, int32_t y, unsigned int font_size, Align align, Layout layout, const char *fmt, ...) {
	char str[256] = {0};
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(str, 256, fmt, ap);
	va_end(ap);

	return calculateBoundingBox(x, y, font_size, align, layout, (const char *)str);
}

FT_BBox OpenFontRender::calculateBoundingBox(int32_t x, int32_t y, unsigned int font_size, Align align, Layout layout, const char *str) {
	FT_Error error;
	FT_BBox bbox;
	size_t tmp_font_size = getFontSize();
	Cursor tmp_cursor    = _cursor;

	_cursor = {x, y};
	setFontSize(font_size);

	switch (layout) {
	case Layout::Horizontal:
		drawHString(str, x, y, 0xFFFF, 0x0000, align, Drawing::Skip, bbox, error);
		break;
	case Layout::Vertical:
		// Not support now
		break;
	default:
		break;
	}

	setFontSize(tmp_font_size);
	_cursor = tmp_cursor;

	return bbox;
}

unsigned int OpenFontRender::calculateFitFontSizeFmt(uint32_t limit_width, uint32_t limit_height, Layout layout, const char *fmt, ...) {
	char str[256] = {0};
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(str, 256, fmt, ap);
	va_end(ap);

	return calculateFitFontSize(limit_width, limit_height, layout, (const char *)str);
}

unsigned int OpenFontRender::calculateFitFontSize(uint32_t limit_width, uint32_t limit_height, Layout layout, const char *str) {
	FT_Error error;
	FT_BBox bbox1        = {0, 0, 0, 0};
	FT_BBox bbox2        = {0, 0, 0, 0};
	size_t tmp_font_size = getFontSize();
	Cursor tmp_cursor    = _cursor;
	unsigned int fs1     = 10;
	unsigned int fs2     = 50;
	int32_t w1, w2, h1, h2;

	// point1
	setFontSize(fs1);
	switch (layout) {
	case Layout::Horizontal:
		drawHString(str, 0, 0, 0xFFFF, 0x0000, Align::Left, Drawing::Skip, bbox1, error);
		break;
	case Layout::Vertical:
		// Not support now
		break;
	default:
		setFontSize(tmp_font_size);
		_cursor = tmp_cursor;
		return 0;
	}
	w1 = bbox1.xMax - bbox1.xMin;
	h1 = bbox1.yMax - bbox1.yMin;

	// point2
	setFontSize(fs2);
	switch (layout) {
	case Layout::Horizontal:
		drawHString(str, 0, 0, 0xFFFF, 0x0000, Align::Left, Drawing::Skip, bbox2, error);
		break;
	case Layout::Vertical:
		// Not support now
		break;
	default:
		setFontSize(tmp_font_size);
		_cursor = tmp_cursor;
		return 0;
	}
	w2 = bbox2.xMax - bbox2.xMin;
	h2 = bbox2.yMax - bbox2.yMin;

	unsigned int wfs = ((fs2 - fs1) / (w2 - w1 + 0.000001)) * (limit_width - w1) + fs1;
	unsigned int hfs = ((fs2 - fs1) / (h2 - h1 + 0.000001)) * (limit_height - h1) + fs1;

	setFontSize(tmp_font_size);
	_cursor = tmp_cursor;

	return std::min(wfs, hfs);
}

void OpenFontRender::showFreeTypeVersion() {
	char s[OFR_FT_VERSION_STRING_SIZE] = {0};
	getFreeTypeVersion(s);
	g_Print(s);
}

void OpenFontRender::showCredit() {
	char s[OFR_CREDIT_STRING_SIZE] = {0};
	getCredit(s);
	g_Print(s);
}

void OpenFontRender::getFreeTypeVersion(char *str) {
	snprintf(str, OFR_FT_VERSION_STRING_SIZE,
	         "FreeType version: %d.%d.%d\n", FREETYPE_MAJOR, FREETYPE_MINOR, FREETYPE_PATCH);
}

void OpenFontRender::getCredit(char *str) {
	snprintf(str, OFR_CREDIT_STRING_SIZE,
	         "Portions of this software are copyright (c) < %d.%d.%d > The FreeTypeProject (www.freetype.org).  All rights reserved.\n",
	         FREETYPE_MAJOR, FREETYPE_MINOR, FREETYPE_PATCH);
}

void OpenFontRender::setDebugLevel(uint8_t level) {
	_debug_level = level;
}

/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/*/
//
//  OpenFontRender Class Public Methods
//  ( Direct calls are deprecated. )
//
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/*/

void OpenFontRender::set_drawPixel(std::function<void(int32_t, int32_t, uint16_t)> user_func) {
	_drawPixel = user_func;
}
void OpenFontRender::set_startWrite(std::function<void(void)> user_func) {
	_startWrite = user_func;
}
void OpenFontRender::set_endWrite(std::function<void(void)> user_func) {
	_endWrite = user_func;
}

/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/*/
//
//  OpenFontRender Class Private Methods
//
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/*/

uint32_t OpenFontRender::getFontMaxHeight() {
	FT_Error error;
	FT_Face face;
	FTC_ScalerRec scaler;
	FT_Size asize                      = NULL;
	static uint32_t max_height         = 0;
	static unsigned int prev_font_size = 0;

	if (prev_font_size == _font.size) {
		return max_height;
	}

	scaler.face_id = (FTC_FaceID)_face_id;
	scaler.width   = 0;
	scaler.height  = _font.size;
	scaler.pixel   = true;
	scaler.x_res   = 0;
	scaler.y_res   = 0;

	// error = FTC_Manager_LookupFace(_ftc_manager, (FTC_FaceID)_face_id, &face);
	error = FTC_Manager_LookupSize(_ftc_manager, &scaler, &asize);
	if (error) {
		return 0;
	}
	face = asize->face;

	int bbox_ymax = FT_MulFix(face->bbox.yMax, face->size->metrics.y_scale) >> 6;
	int bbox_ymin = FT_MulFix(face->bbox.yMin, face->size->metrics.y_scale) >> 6;
	max_height    = (bbox_ymax - bbox_ymin);

	prev_font_size = _font.size;
	return max_height;
}

void OpenFontRender::draw2screen(FT_BitmapGlyph glyph, uint32_t x, uint32_t y, uint16_t fg, uint16_t bg) {
	_startWrite();
	for (size_t _y = 0; _y < glyph->bitmap.rows; ++_y) {
		for (size_t _x = 0; _x < glyph->bitmap.width; ++_x) {
			uint8_t alpha = glyph->bitmap.buffer[_y * glyph->bitmap.pitch + _x];
			debugPrintf((_debug_level & OFR_DEBUG) ? OFR_RAW : OFR_NONE, "%c", (alpha == 0x00 ? ' ' : 'o'));
			if (_transparent_background && alpha == 0x00) {
				continue;
			}
			_drawPixel(_x + x + glyph->left, _y + y - glyph->top, alphaBlend(alpha, fg, bg));
		}
		debugPrintf((_debug_level & OFR_DEBUG) ? OFR_RAW : OFR_NONE, "\n");
	}
	_endWrite();
}

uint16_t OpenFontRender::decodeUTF8(uint8_t *buf, uint16_t *index, uint16_t remaining) {
	uint16_t c = buf[(*index)++];
	//
	// 7 bit Unicode
	if ((c & 0x80) == 0x00)
		return c;

	// 11 bit Unicode
	if (((c & 0xE0) == 0xC0) && (remaining > 1))
		return ((c & 0x1F) << 6) | (buf[(*index)++] & 0x3F);

	// 16 bit Unicode
	if (((c & 0xF0) == 0xE0) && (remaining > 2)) {
		c = ((c & 0x0F) << 12) | ((buf[(*index)++] & 0x3F) << 6);
		return c | ((buf[(*index)++] & 0x3F));
	}

	// 21 bit Unicode not supported so fall-back to extended ASCII
	// if ((c & 0xF8) == 0xF0) return c;

	return c; // fall-back to extended ASCII
}

uint16_t OpenFontRender::color565(uint8_t r, uint8_t g, uint8_t b) {
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

uint16_t OpenFontRender::alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc) {
	// For speed use fixed point maths and rounding to permit a power of 2 division
	uint16_t fgR = ((fgc >> 10) & 0x3E) + 1;
	uint16_t fgG = ((fgc >> 4) & 0x7E) + 1;
	uint16_t fgB = ((fgc << 1) & 0x3E) + 1;

	uint16_t bgR = ((bgc >> 10) & 0x3E) + 1;
	uint16_t bgG = ((bgc >> 4) & 0x7E) + 1;
	uint16_t bgB = ((bgc << 1) & 0x3E) + 1;

	// Shift right 1 to drop rounding bit and shift right 8 to divide by 256
	uint16_t r = (((fgR * alpha) + (bgR * (255 - alpha))) >> 9);
	uint16_t g = (((fgG * alpha) + (bgG * (255 - alpha))) >> 9);
	uint16_t b = (((fgB * alpha) + (bgB * (255 - alpha))) >> 9);

	// Combine RGB565 colours into 16 bits
	// return ((r&0x18) << 11) | ((g&0x30) << 5) | ((b&0x18) << 0); // 2 bit greyscale
	// return ((r&0x1E) << 11) | ((g&0x3C) << 5) | ((b&0x1E) << 0); // 4 bit greyscale
	return (r << 11) | (g << 5) | (b << 0);
}

/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/*/
//
//  Functions
//  (Use only within this file.)
//
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/*/

FT_Error ftc_face_requester(FTC_FaceID face_id, FT_Library library, FT_Pointer request_data, FT_Face *face) {
	FT_Error error;
	FontDataInfo *info = (FontDataInfo *)request_data;

	debugPrintf((info->debug_level & OFR_INFO), "Font load required. FaceId: %d\n", face_id);

	if (info->from == FROM_FILE) {
		debugPrintf((info->debug_level & OFR_INFO), "Load from file.\n");
		const uint8_t FACE_INDEX = 0;

		error = FT_New_Face(library, info->filepath, FACE_INDEX, face); // create face object
		if (error) {
			debugPrintf((info->debug_level & OFR_ERROR), "Font load Failed: 0x%02X\n", error);
		} else {
			debugPrintf((info->debug_level & OFR_INFO), "Font load Success!\n");
		}

	} else if (info->from == FROM_MEMORY) {
		debugPrintf((info->debug_level & OFR_INFO), "Load from memory.\n");
		const uint8_t FACE_INDEX = 0;

		error = FT_New_Memory_Face(library, info->data, info->data_size, FACE_INDEX, face); // create face object
		if (error) {
			debugPrintf((info->debug_level & OFR_ERROR), "Font load Failed: 0x%02X\n", error);
		} else {
			debugPrintf((info->debug_level & OFR_INFO), "Font load Success!\n");
		}
	}
	return error;
}

void debugPrintf(uint8_t level, const char *fmt, ...) {
	char str[256] = {0};
	va_list ap;

	switch (level) {
	case OFR_NONE:
		return;
	case OFR_ERROR: {
		va_start(ap, fmt);
		vsnprintf(str, 256, fmt, ap);
		va_end(ap);

		g_Print("[OFR ERROR] ");
		g_Print(str);
		return;
	}
	case OFR_INFO: {
		va_start(ap, fmt);
		vsnprintf(str, 256, fmt, ap);
		va_end(ap);

		g_Print("[OFR INFO] ");
		g_Print(str);
		return;
	}
	case OFR_DEBUG: {
		va_start(ap, fmt);
		vsnprintf(str, 256, fmt, ap);
		va_end(ap);

		g_Print("[OFR DEBUG] ");
		g_Print(str);
		return;
	}
	case OFR_RAW: {
		va_start(ap, fmt);
		vsnprintf(str, 256, fmt, ap);
		va_end(ap);

		g_Print(str);
		return;
	}
	default:
		return;
	}
}

#ifdef FREERTOS_CONFIG_H
void RenderTask(void *pvParameters) {
	while (g_UseRenderTask == true) {
		if (g_RenderTaskStatus != RENDERING) {
			vTaskDelay(1);
			continue;
		}
		g_TaskParameter.error = FT_Glyph_To_Bitmap(&g_TaskParameter.image, FT_RENDER_MODE_NORMAL, NULL, false);
		g_RenderTaskStatus    = (g_TaskParameter.error == FT_Err_Ok) ? END_SUCCESS : END_ERROR;
	}
	g_RenderTaskHandle = NULL;
	vTaskDelete(NULL);
}
#endif

/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/*/
//
//  Functions
//  ( Direct calls are deprecated. )
//
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/*/

void set_printFunc(std::function<void(const char *)> user_func) {
	g_Print = user_func;
}
