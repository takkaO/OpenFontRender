// -------------------------------------------------------
//  OpenFontRender.h
//
//  Copyright (c) 2021 takkaO
//
//  If you use, modify or redistribute this file as part of
//  the original repository, please follow the repository's license.
//
// -------------------------------------------------------

#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <utility>

class BoundingBox {
public:
	void setBoundingBoxWithCoordinates(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
	void setBoundingBoxWithWidthHeight(int32_t x1, int32_t y1, uint32_t w, uint32_t h);

	int32_t x();
	int32_t y();
	uint32_t width();
	uint32_t height();
	int32_t getEndX();
	int32_t getEndY();

private:
	int32_t _x = 0, _y = 0;
	uint32_t _w = 0, _h = 0;
};

#endif /* BOUNDING_BOX_H */