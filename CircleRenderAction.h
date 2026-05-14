#pragma once

#include <optional>
#include "RenderAction.h"

class CircleRenderAction : public RenderAction {
public:
	CircleRenderAction(const RenderCommand& command) : RenderAction(command) {}
	
protected:
	// Inherited via RenderAction
	void onMouseClick(Input::MouseInput) override;
	
private:
	std::optional<Point> center;
	std::optional<Point> radiusPoint;
};