#pragma once

#include "RenderAction.h"
#include "UtilTypes.h"
#include <vector>

class FillingRenderAction : public RenderAction
{
public:
	FillingRenderAction(const RenderCommand& command) : RenderAction(command) {}

	void onMouseClick(Input::MouseInput) override;

private:
	std::vector<Point> clickedPoints;
};
