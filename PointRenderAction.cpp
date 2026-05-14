#include "PointRenderAction.h"
#include "PointRenderer.h"
#include "ShapeStore.h"
#include "WindowHandler.h"
#include "RenderPipeline.h"
#include <memory>

void PointRenderAction::onMouseClick(Input::MouseInput mi)
{
    logger << "Drawing point at (" << mi.clickedPos.x << ", " << mi.clickedPos.y << ")\n";

    auto pipeline = RenderPipeline::getInstance();
    auto color = pipeline->getDrawingColor();

    auto shape = std::make_unique<PointRenderer>(mi.clickedPos, color);
    auto handler = WindowHandler::getInstance();
    if (handler) {
        InvalidateRect(handler->getHWND(), nullptr, TRUE);
    }

    ShapeStore::addShape(std::move(shape));
    isDone = true;
}
