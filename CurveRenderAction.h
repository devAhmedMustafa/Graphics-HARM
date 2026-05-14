#pragma once

#include "RenderAction.h"

class CurveRenderAction : public RenderAction
{
public:
	CurveRenderAction(const RenderCommand& command) : RenderAction(command) {}
	void onMouseClick(Input::MouseInput) override;

private:
	std::vector<Point> points;
};

