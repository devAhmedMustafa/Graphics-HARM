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

	std::string serialize() const override {
		return std::to_string(seen) + "," + std::to_string(color) + "," + std::to_string(center.x) + "," + std::to_string(center.y) + "," +
			std::to_string(radius);
	}

	std::string getShapeType() const override {
		return "DirectCircle";
	}

	Point center;
	int radius;

};