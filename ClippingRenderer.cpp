#include "ClippingRenderer.h"
#include "ClipAlgorithms.h"
#include "ShapeStore.h"
#include "DDALineRenderer.h"

ClippingRenderer::ClippingRenderer(
	size_t shapeIndexToClip,
	const Point& clipStart,
	const Point& clipEnd,
	ClippingType type,
	COLORREF color
) : ShapeRenderer(color),
	m_shapeIndex(shapeIndexToClip),
	m_clipStart(clipStart),
	m_clipEnd(clipEnd),
	m_clipType(type)
{
}

void ClippingRenderer::draw(HDC hdc)
{
	// Get the shape from the store
	auto shapes = ShapeStore::getShapes();
	if (m_shapeIndex >= shapes.size()) {
		// Shape was removed or index is invalid
		return;
	}

	auto shapeToClip = shapes[m_shapeIndex];
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
	case ClippingType::RectClip_Point:
		// Point clipping against rectangle
		// For a point, we just draw the clipping border
		ClipPointAsRect(hdc, m_clipStart.x, m_clipStart.y, bx1, by1, bx2, by2, color, true);
		break;

	case ClippingType::RectClip_Line:
	{

		auto shapeLine = dynamic_cast<DDALineRenderer*>(shapeToClip);
		ClipLineAsRect(hdc, shapeLine->start.x, shapeLine->start.y, shapeLine->end.x, shapeLine->end.y,
			bx1, by1, bx2, by2, color, true);
		break;
	}

	case ClippingType::RectClip_Polygon:
		// Polygon clipping against rectangle - Sutherland-Hodgman would be used here
		// For now, draw the clipping border
		DrawClipBorder(hdc, bx1, by1, bx2, by2, RGB(180, 0, 0));
		shapeToClip->draw(hdc);
		break;

	case ClippingType::SquareClip_Point:
	{
		// Point clipping against square
		int side = std::abs(m_clipEnd.x - m_clipStart.x);
		ClipPointAsSquare(hdc, m_clipStart.x, m_clipStart.y, m_clipStart.x, m_clipStart.y, 
						  side, color, true);
		break;
	}

	case ClippingType::SquareClip_Line:
	{
		// Line clipping against square
		int side = std::abs(m_clipEnd.x - m_clipStart.x);
		ClipLineAsSquare(hdc, m_clipStart.x, m_clipStart.y, m_clipEnd.x, m_clipEnd.y,
						 m_clipStart.x, m_clipStart.y, side, color, true);
		break;
	}
	}

	// Restore the graphics state
	RestoreDC(hdc, savedDC);
}
