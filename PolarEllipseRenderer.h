#pragma once

#include "ShapeRenderer.h"
#include "ElipseAlgorithms.h"

class PolarEllipseRenderer : public ShapeRenderer
{

public:
	PolarEllipseRenderer(int xc, int yc, int rx, int ry, COLORREF color) : ShapeRenderer(color), xc(xc), yc(yc), rx(rx), ry(ry) {}

	void draw(HDC hdc) override {
		PolarEllipse(hdc, xc, yc, rx, ry, color);
	}

private:
	int xc, yc, rx, ry;

};