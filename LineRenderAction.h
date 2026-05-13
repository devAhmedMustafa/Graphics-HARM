#pragma once

#include "RenderAction.h"

class LineRenderAction : public RenderAction
{
public:
	LineRenderAction(const RenderCommand& command) : RenderAction(command) {}

	void onMouseClick(Input::MouseInput) override;

private:
	std::optional<Point> startPoint;
	std::optional<Point> endPoint;
};

