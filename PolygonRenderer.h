#pragma once

#include "ShapeRenderer.h"
#include "LineAlgorithms.h"

#include "UtilTypes.h"

class PolygonRenderer : public ShapeRenderer
{
public:
	PolygonRenderer(std::vector<Point> points, COLORREF color) : ShapeRenderer(color), points(std::move(points)) {}

	void draw(HDC hdc) override {
		if (points.size() < 2) return;
		for (size_t i = 0; i < points.size(); ++i) {
			const auto& p1 = points[i];
			const auto& p2 = points[(i + 1) % points.size()];
			lineDDA(hdc, p1.x, p1.y, p2.x, p2.y, color);
		}
	}

	std::string serialize() const override {
		std::string result = std::to_string(color) + "," + std::to_string(points.size());
		for (const auto& p : points) {
			result += "," + std::to_string(p.x) + "," + std::to_string(p.y);
		}
		return result;
	}

	std::string getShapeType() const override {
		return "Polygon";
	}

	std::vector<Point> points;
};