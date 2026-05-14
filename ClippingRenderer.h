#pragma once

#include "ShapeRenderer.h"
#include "UtilTypes.h"
#include <memory>

class ClippingRenderer : public ShapeRenderer
{
public:
	enum class ClippingType {
		// Rectangle clipping
		RectClip_Point,
		RectClip_Line,
		RectClip_Polygon,
		// Square clipping
		SquareClip_Point,
		SquareClip_Line
	};

	// Constructor for clipping a stored shape (by index) against a clipping region
	ClippingRenderer(
		size_t shapeIndexToClip,
		const Point& clipStart,
		const Point& clipEnd,
		ClippingType type,
		COLORREF color
	);

	void draw(HDC hdc) override;

	std::string serialize() const override {
		return std::to_string(color) + "," +
			std::to_string(m_shapeIndex) + "," +
			std::to_string(m_clipStart.x) + "," + std::to_string(m_clipStart.y) + "," +
			std::to_string(m_clipEnd.x) + "," + std::to_string(m_clipEnd.y) + "," +
			std::to_string(static_cast<int>(m_clipType));
	}

	std::string getShapeType() const override {
		return "Clipping";
	}

	size_t m_shapeIndex;
	Point m_clipStart;
	Point m_clipEnd;
	ClippingType m_clipType;
};
