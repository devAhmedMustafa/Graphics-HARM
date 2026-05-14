#include "ClippingRenderer.h"
#include "ClipAlgorithms.h"
#include "ShapeStore.h"
#include "DDALineRenderer.h"
#include "Line.h"
#include "PolygonRenderer.h"
#include "PointRenderer.h"

ClippingRenderer::ClippingRenderer(
	size_t shapeToClip,
	const Point& clipStart,
	const Point& clipEnd,
	ClippingType type,
	COLORREF color
) : ShapeRenderer(color),
	shapeToClipIndex(shapeToClip),
	m_clipStart(clipStart),
	m_clipEnd(clipEnd),
	m_clipType(type)
{
}

void ClippingRenderer::draw(HDC hdc)
{
	auto shapes = ShapeStore::getShapes();
	auto shapeToClip = shapes.at(shapeToClipIndex);

	if (!shapeToClip) return;

	// Define clipping region bounds
	int bx1 = min(m_clipStart.x, m_clipEnd.x);
	int by1 = min(m_clipStart.y, m_clipEnd.y);
	int bx2 = max(m_clipStart.x, m_clipEnd.x);
	int by2 = max(m_clipStart.y, m_clipEnd.y);

	// Save the current graphics state
	int savedDC = SaveDC(hdc);

	switch (m_clipType)
	{
	case ClippingType::RectClip_Point: {
		auto shapePoint = dynamic_cast<PointRenderer*>(shapeToClip);
		ClipPointAsRect(hdc, shapePoint->point.x, shapePoint->point.y, bx1, by1, bx2, by2, shapeToClip->color, true);
		break;
	}
	case ClippingType::RectClip_Line:
	{
	
		auto shapeLine = dynamic_cast<Line*>(shapeToClip);
		ClipLineAsRect(hdc, shapeLine->start.x, shapeLine->start.y, shapeLine->end.x, shapeLine->end.y,
			bx1, by1, bx2, by2, shapeToClip->color, true);
			
		break;
	}

	case ClippingType::RectClip_Polygon:
	{
		auto shapePoly = dynamic_cast<PolygonRenderer*>(shapeToClip);
		if (shapePoly) {
			int n = static_cast<int>(shapePoly->points.size());
			if (n >= 2) {
				std::vector<int> xs(n);
				std::vector<int> ys(n);
				for (int i = 0; i < n; ++i) {
					xs[i] = shapePoly->points[i].x;
					ys[i] = shapePoly->points[i].y;
				}
				ClipPolygonAsRect(hdc, xs.data(), ys.data(), n,
					bx1, by1, bx2, by2,
					shapeToClip->color, true);
			}
		}
		else {
			DrawClipBorder(hdc, bx1, by1, bx2, by2, RGB(180, 0, 0));
			shapeToClip->draw(hdc);
		}
		break;
	}

	case ClippingType::SquareClip_Point:
	{
		int side = std::abs(m_clipEnd.x - m_clipStart.x);
		auto shapePoint = dynamic_cast<PointRenderer*>(shapeToClip);
		ClipPointAsSquare(hdc, shapePoint->point.x, shapePoint->point.y, m_clipStart.x, m_clipStart.y,
						  side, shapeToClip->color, true);
		break;
	}

	case ClippingType::SquareClip_Line:
	{
		int side = std::abs(m_clipEnd.x - m_clipStart.x);
		auto shapeLine = dynamic_cast<Line*>(shapeToClip);
		ClipLineAsSquare(hdc, shapeLine->start.x, shapeLine->start.y, shapeLine->end.x, shapeLine->end.y,
						 m_clipStart.x, m_clipStart.y, side, shapeToClip->color, true);
		break;
	}
	}

	// Restore the graphics state
	RestoreDC(hdc, savedDC);
}
