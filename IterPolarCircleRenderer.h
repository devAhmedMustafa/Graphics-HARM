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

	std::string serialize() const override {
		return std::to_string(seen) + "," + std::to_string(color) + "," + std::to_string(center.x) + "," + std::to_string(center.y) + "," +
			std::to_string(radius);
	}

	std::string getShapeType() const override {
		return "IterPolarCircle";
	}

	Point center;
	int radius;
};