#pragma once

#include "ShapeRenderer.h"
#include "UtilTypes.h"
#include "CircleAlgorithms.h"

class DirectCircleRenderer : public ShapeRenderer {

public:
	DirectCircleRenderer(Point center, int radius, COLORREF color) : center(center), radius(radius), ShapeRenderer(color) {}

	void draw(HDC hdc) override {

		DirectCircle(hdc, center.x, center.y, radius, color);
	}

private:
	Point center;
	int radius;

};