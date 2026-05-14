#include "PolygonRenderAction.h"
#include "PolygonRenderer.h"
#include <memory>
#include "ShapeStore.h"
#include "WindowHandler.h"

void PolygonRenderAction::onMouseClick(Input::MouseInput mi)
{
	vertices.push_back(mi.clickedPos);
	logger << "Added vertex (" << mi.clickedPos.x << ", " << mi.clickedPos.y << ")\n";
	if (vertices.size() < expectedVertices) {
		return;
	}

	auto pipeline = RenderPipeline::getInstance();
	auto color = pipeline->getDrawingColor();

	auto shape = std::make_unique<PolygonRenderer>(vertices, color);
	auto handler = WindowHandler::getInstance();
	if (handler) {
		InvalidateRect(handler->getHWND(), nullptr, TRUE);
	}
	ShapeStore::addShape(std::move(shape));
	isDone = true;
}
