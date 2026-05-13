#include "LineRenderAction.h"
#include "ShapeRenderer.h"
#include "DDALineRenderer.h"

#include <memory>
#include "WindowHandler.h"
#include "MidpointLineRenderer.h"
#include "ShapeStore.h"
#include "ParametricLineRenderer.h"

void LineRenderAction::onMouseClick(Input::MouseInput mi)
{
	if (!startPoint) {
		startPoint = mi.clickedPos;
		return;
	}
	else if (!endPoint) {
		endPoint = mi.clickedPos;
	}

	bool isDDA = command.msg == "Algo:DDA";
	bool isMidpoint = command.msg == "Algo:Midpoint";
	bool isParametric = command.msg == "Algo:Parametric";

	std::unique_ptr<ShapeRenderer> shape = nullptr;

	auto handler = WindowHandler::getInstance();
	auto pipeline = RenderPipeline::getInstance();
	auto color = pipeline->getDrawingColor();

	if (isDDA) {
		logger << "Drawing line with DDA algorithm\n";
		shape = std::make_unique<DDALineRenderer>(*startPoint, *endPoint, color);
	}
	else if (isMidpoint) {
		logger << "Drawing line with Midpoint algorithm\n";
		shape = std::make_unique<MidpointLineRenderer>(*startPoint, *endPoint, color);
	}
	else if (isParametric) {
		logger << "Drawing line with Parametric algorithm\n";
		shape = std::make_unique<ParametricLineRenderer>(*startPoint, *endPoint, color);
	}
	else {
		logger << "Unknown line drawing algorithm: " << command.msg << "\n";
		isDone = true;
		return;
	}

	if (shape) {
		InvalidateRect(handler->getHWND(), nullptr, TRUE);
		ShapeStore::addShape(std::move(shape));
		isDone = true;
	}
}
