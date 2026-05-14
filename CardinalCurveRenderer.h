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
			points[0].y,
			points[1].x,
			points[1].y,
			points[2].x,
			points[2].y,
			points[3].x,
			points[3].y,
			color
		);
	}

	std::string serialize() const override {
		return std::to_string(color) + "," +
			std::to_string(points[0].x) + "," + std::to_string(points[0].y) + "," +
			std::to_string(points[1].x) + "," + std::to_string(points[1].y) + "," +
			std::to_string(points[2].x) + "," + std::to_string(points[2].y) + "," +
			std::to_string(points[3].x) + "," + std::to_string(points[3].y);
	}

	std::string getShapeType() const override {
		return "CardinalCurve";
	}

private:
	Point points[4];

};