#pragma once

#include "RenderAction.h"

class EllipseRenderAction : public RenderAction
{
public:
	EllipseRenderAction(const RenderCommand& command) : RenderAction(command) {}

	void onMouseClick(Input::MouseInput) override;

private:
	std::optional<Point> center, majorAxisEnd, minorAxisEnd;
};

