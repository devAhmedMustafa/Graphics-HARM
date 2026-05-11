#pragma once

#include <string>
#include <queue>
#include "RenderCommand.h"

class AppContext {

public:
	std::queue<RenderCommand> channel;
	bool running;

};