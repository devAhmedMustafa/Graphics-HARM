#pragma once

#include "RenderAction.h"

class PolygonRenderAction : public RenderAction
{
public:
	PolygonRenderAction(const RenderCommand& command) : RenderAction(command) {
		expectedVertices = std::stoi(command.msg);
	}
	void onMouseClick(Input::MouseInput mi) override;

private:
	int expectedVertices = 0;
	std::vector<Point> vertices;
};

