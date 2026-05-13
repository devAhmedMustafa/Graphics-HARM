#pragma once

#include <windows.h>

class ShapeRenderer {
public:
	ShapeRenderer(COLORREF color) : color(color) {}
	virtual void draw(HDC) = 0;

protected:
	COLORREF color;
};