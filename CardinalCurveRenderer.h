#pragma once

#include "ShapeRenderer.h"
#include "CurveAlgorithms.h"

#include "UtilTypes.h"
#include <vector>

class CardinalCurveRenderer : public ShapeRenderer
{
public:
	CardinalCurveRenderer(COLORREF color, std::vector<Point> points) : ShapeRenderer(color) {
		for (int i = 0; i < 4; i++) {
			this->points[i] = points[i];
		}
	}

	void draw(HDC hdc) override {
		CardinalSpline(hdc,
			points[0].x,
			points[1].x,
			points[2].x,
			points[3].x,
			points[0].y,
			points[1].y,
			points[2].y,
			points[3].y,
			color
		);
	}

private:
	Point points[4];

};