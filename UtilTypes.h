#pragma once

#include <cmath>

struct Point {
	int x, y;

	int distance(const Point& e) const {
		auto xDiff = x - e.x;
		auto yDiff = y - e.y;

		return (int)sqrt(xDiff * xDiff + yDiff * yDiff);
	}
};