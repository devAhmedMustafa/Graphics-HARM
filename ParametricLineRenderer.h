#pragma once

#include "ShapeRenderer.h"
#include "UtilTypes.h"
#include "LineAlgorithms.h"

class ParametricLineRenderer : public ShapeRenderer {

public:
	ParametricLineRenderer(Point start, Point end, COLORREF color) : start(start), end(end), ShapeRenderer(color) {}

	void draw(HDC hdc) override {
		ParametricLine(hdc, start.x, start.y, end.x, end.y, color);
	}

private:
	Point start, end;
};