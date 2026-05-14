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

	std::string serialize() const override {
		return std::to_string(color) + "," + std::to_string(start.x) + "," + std::to_string(start.y) + "," +
			std::to_string(end.x) + "," + std::to_string(end.y);
	}

	std::string getShapeType() const override {
		return "ParametricLine";
	}

	Point start, end;
};