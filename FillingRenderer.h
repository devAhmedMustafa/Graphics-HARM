#pragma once

#include "ShapeRenderer.h"
#include "UtilTypes.h"
#include "filling.h"

class FillingRenderer : public ShapeRenderer {

public:
	enum class FillingType {
		CircleWithLines,
		CircleWithCircles,
		HermiteCurves,
		BezierCurves,
		ConvexPolygon,
		NonConvexPolygon,
		RecursiveFloodFill,
		NonRecursiveFloodFill
	};

	// Constructor for circle filling
	FillingRenderer(Point center, int radius, int quadrant, FillingType type, COLORREF color)
		: ShapeRenderer(color), fillingType(type), center(center), radius(radius), circleQuadrant(quadrant), isCircle(true) { }

	// Constructor for rectangle/box filling
	FillingRenderer(Point topLeft, Point bottomRight, FillingType type, COLORREF color)
		: ShapeRenderer(color), fillingType(type), topLeft(topLeft), bottomRight(bottomRight), isCircle(false) {}

	// Constructor for polygon filling
	FillingRenderer(Filling::Point pts[], int n, FillingType type, COLORREF color)
		: ShapeRenderer(color), fillingType(type), polygonPoints(pts), polygonPointCount(n), isPolygon(true) {}

	// Constructor for flood fill
	FillingRenderer(Point startPos, FillingType type, COLORREF color)
		: ShapeRenderer(color), fillingType(type), floodFillStart(startPos), isFloodFill(true) {}

	void draw(HDC hdc) override;

	std::string serialize() const override {
		std::string data = std::to_string(color) + "," + std::to_string(static_cast<int>(fillingType)) + ",";
		if (isCircle) {
			data += std::to_string(center.x) + "," + std::to_string(center.y) + "," + std::to_string(radius);
		}
		else if (isPolygon) {
			data += std::to_string(polygonPointCount);
			for (int i = 0; i < polygonPointCount; i++) {
				data += "," + std::to_string(polygonPoints[i].x) + "," + std::to_string(polygonPoints[i].y);
			}
		}
		else if (isFloodFill) {
			data += std::to_string(floodFillStart.x) + "," + std::to_string(floodFillStart.y);
		}
		else {
			data += std::to_string(topLeft.x) + "," + std::to_string(topLeft.y) + "," +
				std::to_string(bottomRight.x) + "," + std::to_string(bottomRight.y);
		}
		return data;
	}

	std::string getShapeType() const override {
		return "Filling";
	}

private:
	FillingType fillingType;
	
	// Circle filling members
	Point center;
	int radius;
	bool isCircle = false;
	int circleQuadrant = 0;

	// Rectangle filling members
	Point topLeft, bottomRight;

	// Polygon filling members
	Filling::Point* polygonPoints = nullptr;
	int polygonPointCount = 0;
	bool isPolygon = false;

	// Flood fill members
	Point floodFillStart;
	bool isFloodFill = false;
};
