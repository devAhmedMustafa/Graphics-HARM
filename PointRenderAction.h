#pragma once

#include "RenderAction.h"

class PointRenderAction : public RenderAction
{
public:
    PointRenderAction(const RenderCommand& command) : RenderAction(command) {}
    void onMouseClick(Input::MouseInput mi) override;
};
