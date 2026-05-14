#include "FillingRenderer.h"

void FillingRenderer::draw(HDC hdc)
{
	switch (fillingType)
	{
	case FillingType::CircleWithLines:
		if (isCircle) {
			Filling::FillQuadrant(hdc, center.x, center.y, radius, circleQuadrant);
		}
		break;

	case FillingType::CircleWithCircles:
		if (isCircle) {
			Filling::DrawCircle(hdc, center.x, center.y, radius, color);
			// Additional logic to fill with circles would go here
		}
		break;

	case FillingType::HermiteCurves:
		if (!isCircle) {
			Filling::FillHermite(hdc, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
		}
		break;

	case FillingType::BezierCurves:
		if (!isCircle) {
			Filling::FillBezierHorizontal(hdc, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
		}
		break;

	case FillingType::ConvexPolygon:
		if (isPolygon && polygonPoints && polygonPointCount > 0) {
			Filling::Convex_Fill(hdc, polygonPoints, polygonPointCount, color);
		}
		break;

	case FillingType::NonConvexPolygon:
		if (isPolygon && polygonPoints && polygonPointCount > 0) {
			Filling::Fill_NonConvex(hdc, polygonPoints, polygonPointCount, color);
		}
		break;

	case FillingType::RecursiveFloodFill:
		if (isFloodFill) {
			// Get background color (typically white for unfilled areas)
			COLORREF bgColor = RGB(255, 255, 255);
			Filling::MyFloodFill(hdc, floodFillStart.x, floodFillStart.y, bgColor, color);
		}
		break;

	case FillingType::NonRecursiveFloodFill:
		if (isFloodFill) {
			// Get background color
			COLORREF bgColor = RGB(255, 255, 255);
			Filling::non_recursive_flood_fill(hdc, floodFillStart.x, floodFillStart.y, bgColor, color);
		}
		break;

	default:
		break;
	}
}
