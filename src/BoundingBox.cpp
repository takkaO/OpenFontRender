#include "BoundingBox.h"

void BoundingBox::setBoundingBoxWithCoordinates(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
	if (x1 > x2) {
		std::swap(x1, x2);
	}
	if (y1 > y2) {
		std::swap(y1, y2);
	}
	_x = x1;
	_y = y1;
	_w = (x2 - x1);
	_h = (y2 - y1);
}

void BoundingBox::setBoundingBoxWithWidthHeight(int32_t x1, int32_t y1, uint32_t w, uint32_t h) {
	_x = x1;
	_y = y1;
	_w = w;
	_h = h;
}

int32_t BoundingBox::x() {
	return _x;
}

int32_t BoundingBox::y() {
	return _y;
}

uint32_t BoundingBox::width() {
	return _w;
}

uint32_t BoundingBox::height() {
	return _h;
}

int32_t BoundingBox::getEndX() {
	return _x + _w;
}

int32_t BoundingBox::getEndY() {
	return _y + _h;
}