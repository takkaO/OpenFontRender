#include "OpenFontRender.h"

/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_//_/_/_/_/_/_/_/_/_/*/
//
//  Data Structure Definition
//  (Use only within this file.)
//
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_//_/_/_/_/_/_/_/_/_/*/

enum LoadFontFrom {
	FROM_FILE,
	FROM_MEMORY
};

typedef struct {
	enum LoadFontFrom from;	// data source
	char *filepath;			// ttf file path
	unsigned char *data;	// ttf array
	size_t data_size;		// ttf array size
	uint8_t debug_level;	// debug level
} FontDataInfo;

#ifdef FREERTOS_CONFIG_H
enum RenderTaskStatus {
	IDLE,
	LOCK,
	RENDERING,
	END_ERROR,
	END_SUCCESS
};

typedef struct {
	enum RenderMode mode;			// Rendering mode
	FTC_Manager *ftc_manager;		// ftc_manager obj
	FTC_SBitCache *ftc_sbit_cache;	// ftc_sbit_cache obj
	uint8_t face_id;				// face id
	size_t font_size;				// font size
	FT_UInt glyph_index;			// glyph index
	FTC_SBit *sbit;					// result
	FT_Error error;					// ft_error
	uint8_t debug_level;			// debug level
} RenderTaskParameter;
#endif

/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_//_/_/_/_/_/_/_/_/_/*/
//
//  Function Definition
//  (See below more ditails.)
//
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_//_/_/_/_/_/_/_/_/_/*/

FT_Error ftc_face_requester(FTC_FaceID face_id,
                            FT_Library library,
                            FT_Pointer request_data,
                            FT_Face *face);

void debugPrintf(uint8_t level, const char *fmt, ...);

#ifdef FREERTOS_CONFIG_H
void RenderTask(void *pvParameters);
#endif

/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_//_/_/_/_/_/_/_/_/_/*/
//
//  Global Variables
//  (Use only within this file.)
//
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_//_/_/_/_/_/_/_/_/_/*/

FT_Library g_FtLibrary;
uint8_t g_AvailableFaceId = 0;
bool g_NeedInitialize     = true;

#ifdef FREERTOS_CONFIG_H
TaskHandle_t g_RenderTaskHandle                   = NULL;
volatile enum RenderTaskStatus g_RenderTaskStatus = IDLE;
volatile bool g_UseRenderTask                     = (FREETYPE_MAJOR == 2 && FREETYPE_MINOR == 4 && FREETYPE_PATCH == 12) ? false : true;
RenderTaskParameter g_TaskParameter;
#endif

/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_//_/_/_/_/_/_/_/_/_/*/
//
//  OpenFontRender Class Public Methods
//
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_//_/_/_/_/_/_/_/_/_/*/

OpenFontRender::OpenFontRender() {
	// TODO: Automatic support some micro computers
	_drawPixel = [](int x, int y, int c) {
		static bool flag = true;
		if (flag) {
			Serial.println("** [Warning] Please set drawPixel() using setDrawPixel(). **");
			flag = false;
		}
		return;
	};
	_startWrite = []() { return; };
	_endWrite   = []() { return; };

	_font.size     = 44;     // Set default font size
	_font.fg_color = 0xFFFF; // Set default font color (White)
	_font.bg_color = 0x0000; // Set default background color (Black)
	_debug_level   = OFR_NONE;

	_render_mode = NORMAL;
}

void OpenFontRender::setUseRenderTask(bool enable) {
#ifdef FREERTOS_CONFIG_H
	g_UseRenderTask = enable;
#endif
}

void OpenFontRender::setRenderTaskMode(enum RenderMode mode) {
	_render_mode = mode;
}

void OpenFontRender::setCursor(uint32_t x, uint32_t y) {
	_cursor = {x, y};
}

uint32_t OpenFontRender::getCursorX() {
	return _cursor.x;
}

uint32_t OpenFontRender::getCursorY() {
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

void OpenFontRender::setFontSize(size_t new_size) {
	_font.size = new_size;
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
	error = FTC_Manager_New(g_FtLibrary, 0, 0, 0, &ftc_face_requester, &info, &_ftc_manager);
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

	error = FTC_SBitCache_New(_ftc_manager, &_ftc_sbit_cache);
	if (error) {
		debugPrintf((_debug_level & OFR_ERROR), "FTC_SBitCache_New error: 0x%02X\n", error);
		return error;
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
	error = FTC_Manager_New(g_FtLibrary, 0, 0, 0, &ftc_face_requester, &info, &_ftc_manager);
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

	error = FTC_SBitCache_New(_ftc_manager, &_ftc_sbit_cache);
	if (error) {
		debugPrintf((_debug_level & OFR_ERROR), "FTC_SBitCache_New error: 0x%02X\n", error);
		return error;
	}

	return FT_Err_Ok;
}

FT_Error OpenFontRender::drawChar(uint16_t unicode, uint32_t x, uint32_t y, uint16_t fg, uint16_t bg) {
	FT_Face face;
	FT_Error error;
	FTC_SBit _sbit;
	FTC_ImageTypeRec image_type;
	_cursor = {x, y};

	error = FTC_Manager_LookupFace(_ftc_manager, (FTC_FaceID)_face_id, &face);
	if (error) {
		return error;
	}

	FT_Int cmap_index   = FT_Get_Charmap_Index(face->charmap);
	FT_UInt glyph_index = FTC_CMapCache_Lookup(_ftc_cmap_cache,
	                                           (FTC_FaceID)_face_id,
	                                           cmap_index,
	                                           unicode);

#ifdef FREERTOS_CONFIG_H
	if (g_UseRenderTask) {
		if (g_RenderTaskHandle == NULL) {
			debugPrintf((_debug_level & OFR_INFO), "Create render task\n");
			const uint8_t RUNNING_CORE = 1;
			const uint8_t PRIORITY     = 1;
			xTaskCreateUniversal(RenderTask,
			                     "RenderTask",
			                     8192 + 2048, // It seems that larger than 1024 is better.
			                     NULL,
			                     PRIORITY,
			                     &g_RenderTaskHandle,
			                     RUNNING_CORE);
		}

		while (g_RenderTaskStatus != IDLE) {
			vTaskDelay(1);
		}
		g_RenderTaskStatus = LOCK;

		g_TaskParameter.mode           = _render_mode;
		g_TaskParameter.ftc_manager    = &_ftc_manager;
		g_TaskParameter.face_id        = _face_id;
		g_TaskParameter.font_size      = _font.size;
		g_TaskParameter.glyph_index    = glyph_index;
		g_TaskParameter.debug_level    = _debug_level;
		g_TaskParameter.sbit           = &_sbit;
		g_TaskParameter.ftc_sbit_cache = &_ftc_sbit_cache; // for WITH_CACHE mode

		g_RenderTaskStatus = RENDERING;
		while (g_RenderTaskStatus == RENDERING) {
			vTaskDelay(1);
		}

		if (g_RenderTaskStatus == END_ERROR) {
			g_RenderTaskStatus = IDLE;
			debugPrintf((_debug_level & OFR_ERROR), "Render task error: 0x%02X\n", g_TaskParameter.error);
			return g_TaskParameter.error;
		}

	} else {
		image_type.face_id = (FTC_FaceID)_face_id;
		image_type.width   = 0;
		image_type.height  = _font.size;
		image_type.flags   = FT_LOAD_RENDER;
		error              = FTC_SBitCache_Lookup(_ftc_sbit_cache, &image_type, glyph_index, &_sbit, NULL);
		if (error) {
			debugPrintf((_debug_level & OFR_ERROR), "FTC_SBitCache_Lookup error: 0x%02X\n", error);
			return error;
		}
	}

#else
	image_type.face_id = (FTC_FaceID)_face_id;
	image_type.width   = 0;
	image_type.height  = _font.size;
	image_type.flags   = FT_LOAD_RENDER;
	error              = FTC_SBitCache_Lookup(_ftc_sbit_cache, &image_type, glyph_index, &_sbit, NULL);
	if (error) {
		debugPrintf((_debug_level & OFR_ERROR), "FTC_SBitCache_Lookup error: 0x%02X\n", error);
		return error;
	}
#endif

	// Set the drawing coordinate of the character to the upper left.
	y += (face->bbox.yMax - face->bbox.yMin) * _font.size / face->units_per_EM;

	draw2screen(_sbit, x, y, fg, bg);
	_cursor.x += _sbit->xadvance; // Seek cursor
	_cursor.y += _sbit->yadvance; // Seek cursor

#ifdef FREERTOS_CONFIG_H
	g_RenderTaskStatus = IDLE;
#endif

	return FT_Err_Ok;
}

uint16_t OpenFontRender::drawString(const char *str, uint32_t x, uint32_t y, uint16_t fg, uint16_t bg) {
	FT_Error error;
	return drawString(str, x, y, fg, bg, &error);
}

uint16_t OpenFontRender::drawString(const char *str, uint32_t x, uint32_t y, uint16_t fg, uint16_t bg, FT_Error *error) {
	uint16_t len            = strlen(str);
	uint16_t n              = 0;
	uint16_t wrote_char_num = 0;
	uint32_t max_height;
	_cursor = {x, y};

	{
		FT_Face face;
		*error = FTC_Manager_LookupFace(_ftc_manager, (FTC_FaceID)_face_id, &face);
		if (*error) {
			return wrote_char_num;
		}
		max_height = (face->bbox.yMax - face->bbox.yMin) * _font.size / face->units_per_EM;
	}

	while (n < len) {
		uint16_t unicode = decodeUTF8((uint8_t *)str, &n, len - n);

		if (unicode == '\r') {
			_cursor.x = 0;
			wrote_char_num++;
			continue;
		}
		if (unicode == '\n') {
			_cursor.x = 0;
			_cursor.y += max_height;
			wrote_char_num++;
			continue;
		}

		*error = drawChar(unicode, _cursor.x, _cursor.y, fg, bg);
		if (*error) {
			return wrote_char_num;
		}
		wrote_char_num++;
	}
	return wrote_char_num;
}

uint16_t OpenFontRender::printf(const char *fmt, ...) {
	char str[256] = {0};
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(str, 256, fmt, ap);
	va_end(ap);

	return drawString(str, _cursor.x, _cursor.y, _font.fg_color, _font.bg_color);
}

void OpenFontRender::showFreeTypeVersion(Print &output) {
	String str = "FreeType version: ";
	str += String(FREETYPE_MAJOR) + ".";
	str += String(FREETYPE_MINOR) + ".";
	str += String(FREETYPE_PATCH) + "\n";

	output.print(str.c_str());
}

void OpenFontRender::showCredit(Print &output) {
	String str = "Portions of this software are copyright © < ";
	str += String(FREETYPE_MAJOR) + ".";
	str += String(FREETYPE_MINOR) + ".";
	str += String(FREETYPE_PATCH) + " ";
	str += "> The FreeTypeProject (www.freetype.org).  All rights reserved.\n";

	output.print(str.c_str());
}

void OpenFontRender::setDebugLevel(uint8_t level) {
	_debug_level = level;
}

/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_//_/_/_/_/_/_/_/_/_/*/
//
//  OpenFontRender Class Public Methods
//  ( Direct calls are deprecated. )
//
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_//_/_/_/_/_/_/_/_/_/*/

void OpenFontRender::set_drawPixel(std::function<void(int32_t, int32_t, uint16_t)> user_func) {
	_drawPixel = user_func;
}
void OpenFontRender::set_startWrite(std::function<void(void)> user_func) {
	_startWrite = user_func;
}
void OpenFontRender::set_endWrite(std::function<void(void)> user_func) {
	_endWrite = user_func;
}

/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_//_/_/_/_/_/_/_/_/_/*/
//
//  OpenFontRender Class Private Methods
//
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_//_/_/_/_/_/_/_/_/_/*/

void OpenFontRender::draw2screen(FTC_SBit sbit, uint32_t x, uint32_t y, uint16_t fg, uint16_t bg) {
	_startWrite();
	for (size_t _y = 0; _y < sbit->height; ++_y) {
		for (size_t _x = 0; _x < sbit->width; ++_x) {
			uint8_t alpha = sbit->buffer[_y * sbit->pitch + _x];
			debugPrintf((_debug_level & OFR_DEBUG) ? OFR_RAW : OFR_NONE, "%c", (alpha == 0x00 ? ' ' : 'o'));
			_drawPixel(_x + x + sbit->left, _y + y - sbit->top, alphaBlend(alpha, fg, bg));
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
	//return ((r&0x18) << 11) | ((g&0x30) << 5) | ((b&0x18) << 0); // 2 bit greyscale
	//return ((r&0x1E) << 11) | ((g&0x3C) << 5) | ((b&0x1E) << 0); // 4 bit greyscale
	return (r << 11) | (g << 5) | (b << 0);
}

/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_//_/_/_/_/_/_/_/_/_/*/
//
//  Functions
//  (Use only within this file.)
//
/*_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_//_/_/_/_/_/_/_/_/_/*/

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

		Serial.print("[OFR ERROR] ");
		Serial.print(str);
		return;
	}
	case OFR_INFO: {
		va_start(ap, fmt);
		vsnprintf(str, 256, fmt, ap);
		va_end(ap);

		Serial.print("[OFR INFO] ");
		Serial.print(str);
		return;
	}
	case OFR_DEBUG: {
		va_start(ap, fmt);
		vsnprintf(str, 256, fmt, ap);
		va_end(ap);

		Serial.print("[OFR DEBUG] ");
		Serial.print(str);
		return;
	}
	case OFR_RAW: {
		va_start(ap, fmt);
		vsnprintf(str, 256, fmt, ap);
		va_end(ap);

		Serial.print(str);
		return;
	}
	default:
		return;
	}
}

#ifdef FREERTOS_CONFIG_H
void RenderTask(void *pvParameters) {
	FTC_ImageTypeRec image_type;
	image_type.width = 0;
	image_type.flags = FT_LOAD_RENDER;

	while (g_UseRenderTask == true) {
		if (g_RenderTaskStatus != RENDERING) {
			vTaskDelay(1);
			continue;
		}

		switch (g_TaskParameter.mode) {
		case NORMAL: {
			debugPrintf((g_TaskParameter.debug_level & OFR_INFO), "Render task start: Mode NORMAL\n");

			FT_Face face;
			FTC_SBitRec sbit;
			FTC_ScalerRec ft_scaler;
			FT_Size ft_size;
			vTaskDelay(1); // Important delay to prevent stack overflow
			g_TaskParameter.error = FTC_Manager_LookupFace(*g_TaskParameter.ftc_manager,
			                                               (FTC_FaceID)g_TaskParameter.face_id,
			                                               &face);
			if (g_TaskParameter.error) {
				debugPrintf((g_TaskParameter.debug_level & OFR_ERROR), "Render task error in LookupFace\n");
				break;
			}

			// set scaler parameters
			ft_scaler.face_id = (FTC_FaceID)g_TaskParameter.face_id;
			ft_scaler.width   = 0;
			ft_scaler.height  = g_TaskParameter.font_size;
			ft_scaler.pixel   = true;
			ft_scaler.x_res   = 0;
			ft_scaler.y_res   = 0;
			vTaskDelay(1); // Important delay to prevent stack overflow
			g_TaskParameter.error = FTC_Manager_LookupSize(*g_TaskParameter.ftc_manager, &ft_scaler, &ft_size);
			if (g_TaskParameter.error) {
				debugPrintf((g_TaskParameter.debug_level & OFR_ERROR), "Render task error in LookupSize\n");
				break;
			}
			face = ft_size->face;

			vTaskDelay(1); // Important delay to prevent stack overflow
			g_TaskParameter.error = FT_Load_Glyph(face, g_TaskParameter.glyph_index, FT_LOAD_DEFAULT);
			if (g_TaskParameter.error) {
				debugPrintf((g_TaskParameter.debug_level & OFR_ERROR), "Render task error in Load_Glyph\n");
				break;
			}

			vTaskDelay(1); // Important delay to prevent stack overflow
			g_TaskParameter.error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
			if (g_TaskParameter.error) {
				debugPrintf((g_TaskParameter.debug_level & OFR_ERROR), "Render task error in Render_Glyph\n");
				break;
			}

			/* Copy render bitmap to sbit structure */
			sbit = {
			    face->glyph->bitmap.width,      // width
			    face->glyph->bitmap.rows,       // height
			    face->glyph->bitmap_left,       // left
			    face->glyph->bitmap_top,        // top
			    face->glyph->bitmap.pixel_mode, // format
			    face->glyph->bitmap.num_grays,  // max_gray
			    face->glyph->bitmap.pitch,      // pitch
			    face->glyph->advance.x >> 6,    // xadvance
			    face->glyph->advance.y >> 6,    // yadvance
			    face->glyph->bitmap.buffer      // buffer
			};

			vTaskDelay(1); // Important delay to prevent stack overflow
			*g_TaskParameter.sbit = &sbit;
			vTaskDelay(1); // Important delay to prevent stack overflow
		} break;
		case WITH_CACHE: {
			debugPrintf((g_TaskParameter.debug_level & OFR_INFO), "Render task start: Mode WITH_CACHE\n");

			image_type.face_id = (FTC_FaceID)g_TaskParameter.face_id;
			image_type.height  = g_TaskParameter.font_size;
			vTaskDelay(1); // Important delay to prevent stack overflow
			g_TaskParameter.error = FTC_SBitCache_Lookup(*g_TaskParameter.ftc_sbit_cache,
			                                             &image_type,
			                                             g_TaskParameter.glyph_index,
			                                             g_TaskParameter.sbit,
			                                             NULL);
			vTaskDelay(1); // Important delay to prevent stack overflow
		} break;
		default:
			g_TaskParameter.error = FT_Err_Invalid_Argument;
		}

		debugPrintf((g_TaskParameter.debug_level & OFR_INFO), "Render task Finish\n");
		g_RenderTaskStatus = (g_TaskParameter.error == FT_Err_Ok) ? END_SUCCESS : END_ERROR;

		break;
	}

	g_RenderTaskHandle = NULL;
	vTaskDelete(NULL);
}
#endif
