#pragma once

#include "UtilTypes.h"

class Line {
public:
	Point start, end;

	Line(Point start = Point(), Point end = Point()) : start(start), end(end) {}
};