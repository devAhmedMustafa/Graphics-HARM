#pragma once

#include "ShapeRenderer.h"
#include "Line.h"
#include "UtilTypes.h"
#include "LineAlgorithms.h"

class ParametricLineRenderer : public ShapeRenderer, public Line {

public:
	ParametricLineRenderer(Point start, Point end, COLORREF color) : ShapeRenderer(color), Line(start, end) {}

	void draw(HDC hdc) override {
		ParametricLine(hdc, start.x, start.y, end.x, end.y, color);
	}

	std::string serialize() const override {
		return std::to_string(color) + "," + std::to_string(start.x) + "," + std::to_string(start.y) + "," +
			std::to_string(end.x) + "," + std::to_string(end.y);
	}

	std::string getShapeType() const override {
		return "ParametricLine";
	}
};