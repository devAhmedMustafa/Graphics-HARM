#pragma once
#include <Windows.h>
#include "RenderCommand.h"
#include "Logger.h"
#include "RenderPipeline.h"
#include "Input.h"

class RenderAction {

public:
	RenderAction(const RenderCommand& command) : command(command) {
		Input::startCaputring();
		callbackId = Input::registerMouseClickCallback([this](Input::MouseInput mi) {
			this->onMouseClick(mi);
		});
	}
	virtual ~RenderAction() {
		Input::unregisterMouseClickCallback(callbackId);
	}

	virtual void onMouseClick(Input::MouseInput) = 0;

	bool isDone = false;

protected:
	RenderCommand command;
	Logger logger;
private:
	size_t callbackId = 0;
};