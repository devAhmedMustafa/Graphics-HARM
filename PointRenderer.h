#pragma once

#include <windows.h>
#include <string>
#include "ShapeRenderer.h"
#include "UtilTypes.h"

class PointRenderer : public ShapeRenderer
{
public:
    PointRenderer(const Point& point, COLORREF color)
        : ShapeRenderer(color), point(point) {}

    void draw(HDC hdc) override {
        SetPixel(hdc, point.x, point.y, color);
    }

    std::string serialize() const override {
        return std::to_string(color) + "," + std::to_string(point.x) + "," + std::to_string(point.y);
    }

    std::string getShapeType() const override {
        return "Point";
    }

    Point point;
};
