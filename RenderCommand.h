#pragma once

#include "RenderMode.h"
#include <string>

struct RenderCommand {
	std::string msg;
	RenderMode mode;
};