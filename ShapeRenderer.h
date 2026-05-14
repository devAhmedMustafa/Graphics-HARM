#pragma once

#include <windows.h>
#include <string>
#include <vector>

class ShapeRenderer {
public:
	ShapeRenderer(COLORREF color) : color(color) {}
	virtual ~ShapeRenderer() = default;
	
	virtual void draw(HDC) = 0;

	virtual std::string serialize() const = 0;
	virtual std::string getShapeType() const = 0;

protected:
	COLORREF color;
};