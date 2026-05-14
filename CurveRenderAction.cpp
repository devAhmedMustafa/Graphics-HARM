#include "CurveRenderAction.h"
#include "CardinalCurveRenderer.h"
#include "ShapeStore.h"
#include "WindowHandler.h"

void CurveRenderAction::onMouseClick(Input::MouseInput mi)
{

	if (points.size() < 4) {
		points.push_back(mi.clickedPos);
		logger << "Added point (" << mi.clickedPos.x << ", " << mi.clickedPos.y << ")\n";
	}

	if (points.size() < 4) {
		return;
	}

	auto pipeline = RenderPipeline::getInstance();
	auto color = pipeline->getDrawingColor();

	auto shape = std::make_unique<CardinalCurveRenderer>(color, points);

	auto handler = WindowHandler::getInstance();

	InvalidateRect(handler->getHWND(), nullptr, TRUE);
	ShapeStore::addShape(std::move(shape));

	isDone = true;
}
