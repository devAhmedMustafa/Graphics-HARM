#pragma once
#pragma once

#include "ShapeRenderer.h"
#include "CircleAlgorithms.h"

#include "UtilTypes.h"

class IterPolarCircleRenderer : public ShapeRenderer
{
public:
	IterPolarCircleRenderer(COLORREF color, Point center, int radius) : ShapeRenderer(color), center(center), radius(radius) {}
	void draw(HDC hdc) override {
		IterativePolarCircle(hdc, center.x, center.y, radius, color);
	}

private:
	Point center;
	int radius;
};