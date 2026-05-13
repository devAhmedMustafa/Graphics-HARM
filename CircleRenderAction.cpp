#include "CircleRenderAction.h"

#include "Input.h"
#include "RenderPipeline.h"
#include <string>
#include "WindowHandler.h"
#include "DirectCircleRenderer.h"
#include "ShapeStore.h"

void CircleRenderAction::onMouseClick(Input::MouseInput mi)
{
    if (!center.has_value()) {
        center = mi.clickedPos;
        logger << "Circle center set at (" << center->x << ", " << center->y << ")\n";
        return;
    }
    else if (!radiusPoint.has_value()) {
        radiusPoint = mi.clickedPos;
        logger << "Circle radius point set at (" << radiusPoint->x << ", " << radiusPoint->y << ")\n";
	}

    std::string msg = command.msg;
    bool isDirect = (msg.find("Direct") != std::string::npos) ||
                    (msg.find("Alg:Direct") != std::string::npos) ||
                    (msg.find("Algo:Direct") != std::string::npos);

    if (isDirect) {
		logger << "Drawing circle using Direct Algorithm\n";

		auto handler = WindowHandler::getInstance();

        auto radius = center->distance(*radiusPoint);

		auto shape = std::make_unique<DirectCircleRenderer>(center.value(), radius, RenderPipeline::getInstance()->getDrawingColor());
		ShapeStore::addShape(std::move(shape));
        InvalidateRect(handler->getHWND(), NULL, TRUE);
        isDone = true;
    }
    else {
        logger << "Unsupported circle algorithm: " << msg << "\n";
		isDone = true;
    }
}
