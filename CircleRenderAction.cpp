#include "CircleRenderAction.h"

#include "Input.h"
#include "RenderPipeline.h"
#include <string>
#include "WindowHandler.h"
#include "DirectCircleRenderer.h"
#include "ShapeStore.h"
#include "PolarCircleRenderer.h"
#include "IterPolarCircleRenderer.h"
#include "MidPointCircleRenderer.h"
#include "ModMidCircleRenderer.h"

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
	bool isDirect = msg == "Algo:Direct";
    bool isPolar = msg == "Algo:Polar";
	bool isIterPolar = msg == "Algo:IterativePolar";
	bool isMidPoint = msg == "Algo:Midpoint";
	bool isModifiedMidPoint = msg == "Algo:ModifiedMidpoint";

	auto radius = center->distance(*radiusPoint);
	std::unique_ptr<ShapeRenderer> shape = nullptr;

	auto handler = WindowHandler::getInstance();
    if (isDirect) {
		logger << "Drawing circle using Direct Algorithm\n";
		shape = std::make_unique<DirectCircleRenderer>(center.value(), radius, RenderPipeline::getInstance()->getDrawingColor());
    }
    else if (isPolar) {
		logger << "Drawing circle using Polar Algorithm\n";
		shape = std::make_unique<PolarCircleRenderer>(RenderPipeline::getInstance()->getDrawingColor(), center.value(), radius);
    }
    else if (isIterPolar) {
		logger << "Drawing circle using Iterative Polar Algorithm\n";
		shape = std::make_unique<IterPolarCircleRenderer>(RenderPipeline::getInstance()->getDrawingColor(), center.value(), radius);
    }
    else if (isMidPoint) {
        logger << "Drawing circle using Midpoint Algorithm\n";
        shape = std::make_unique<MidPointCircleRenderer>(RenderPipeline::getInstance()->getDrawingColor(), center.value(), radius);
    }
    else if (isModifiedMidPoint) {
        logger << "Drawing circle using Modified Midpoint Algorithm\n";
        shape = std::make_unique<ModMidCircleRenderer>(RenderPipeline::getInstance()->getDrawingColor(), center.value(), radius);
	}
    else {
        logger << "Unsupported circle algorithm: " << msg << "\n";
		isDone = true;
        return;
    }

    if (shape) {
		InvalidateRect(handler->getHWND(), nullptr, TRUE);
        ShapeStore::addShape(std::move(shape));
        isDone = true;
    }
}
