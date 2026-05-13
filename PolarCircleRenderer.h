#pragma once

#include "ShapeRenderer.h"
#include "CircleAlgorithms.h"

#include "UtilTypes.h"

class PolarCircleRenderer : public ShapeRenderer
{
	public:
	PolarCircleRenderer(COLORREF color, Point center, int radius) : ShapeRenderer(color), center(center), radius(radius) {}
	void draw(HDC hdc) override {
		PolarCircle(hdc, center.x, center.y, radius, color);
	}

private:
	Point center;
	int radius;
};