#pragma once

#include "ShapeRenderer.h"
#include "ElipseAlgorithms.h"

class DirectEllipseRenderer : public ShapeRenderer
{

public:
	DirectEllipseRenderer(int xc, int yc, int rx, int ry, COLORREF color) : ShapeRenderer(color), xc(xc), yc(yc), rx(rx), ry(ry) {}

	void draw(HDC hdc) override {
		DirectEllipse(hdc, xc, yc, rx, ry, color);
	}

	std::string serialize() const override {
		return std::to_string(color) + "," + std::to_string(xc) + "," + std::to_string(yc) + "," +
			std::to_string(rx) + "," + std::to_string(ry);
	}

	std::string getShapeType() const override {
		return "DirectEllipse";
	}

	int xc, yc, rx, ry;

};