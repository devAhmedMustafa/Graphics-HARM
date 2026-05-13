#pragma once

#pragma once

#include "ShapeRenderer.h"
#include "CircleAlgorithms.h"

#include "UtilTypes.h"

class MidPointCircleRenderer : public ShapeRenderer
{
public:
	MidPointCircleRenderer(COLORREF color, Point center, int radius) : ShapeRenderer(color), center(center), radius(radius) {}
	void draw(HDC hdc) override {
		MidpointCircle(hdc, center.x, center.y, radius, color);
	}

private:
	Point center;
	int radius;
};