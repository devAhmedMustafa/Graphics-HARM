#pragma once

#include "RenderAction.h"
#include "UtilTypes.h"
#include <vector>

class ClippingRenderAction : public RenderAction
{
public:
	ClippingRenderAction(const RenderCommand& command) : RenderAction(command) {}

	void onMouseClick(Input::MouseInput) override;

private:
	std::vector<Point> clickedPoints;
};
