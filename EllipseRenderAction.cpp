#include "EllipseRenderAction.h"

#include "ShapeRenderer.h"
#include "DirectEllipseRenderer.h"
#include "PolarEllipseRenderer.h"
#include "MidpointEllipseRenderer.h"
#include "ShapeStore.h"
#include "WindowHandler.h"

void EllipseRenderAction::onMouseClick(Input::MouseInput mi)
{
	if (!center) {
		center = mi.clickedPos;
		logger << "Center set at (" << center->x << ", " << center->y << ")\n";
		return;
	}
	else if (!majorAxisEnd) {
		majorAxisEnd = mi.clickedPos;
		logger << "Major axis end set at (" << majorAxisEnd->x << ", " << majorAxisEnd->y << ")\n";
		return;
	}
	else if (!minorAxisEnd) {
		minorAxisEnd = mi.clickedPos;
		logger << "Minor axis end set at (" << minorAxisEnd->x << ", " << minorAxisEnd->y << ")\n";
	}

	auto rx = center->distance(*majorAxisEnd);
	auto ry = center->distance(*minorAxisEnd);

	bool isDirect = command.msg == "Algo:Direct";
	bool isPolar = command.msg == "Algo:Polar";
	bool isMidpoint = command.msg == "Algo:Midpoint";

	auto pipeline = RenderPipeline::getInstance();
	auto color = pipeline->getDrawingColor();

	std::unique_ptr<ShapeRenderer> shape = nullptr;

	if (isDirect) {
		shape = std::make_unique<DirectEllipseRenderer>(center->x, center->y, rx, ry, color);
	}
	else if (isPolar) {
		shape = std::make_unique<PolarEllipseRenderer>(center->x, center->y, rx, ry, color);
	}
	else if (isMidpoint) {
		shape = std::make_unique<MidpointEllipseRenderer>(center->x, center->y, rx, ry, color);
	}
	else {
		logger << "Unknown algorithm: " << command.msg << "\n";
		isDone = true;
		return;
	}

	if (shape) {
		auto handler = WindowHandler::getInstance();
		InvalidateRect(handler->getHWND(), NULL, TRUE);
		ShapeStore::addShape(std::move(shape));
		logger << "Ellipse added to render pipeline\n";
	}
	else {
		logger << "Failed to create shape renderer\n";
	}
}
