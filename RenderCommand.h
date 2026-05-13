#pragma once

#include "RenderMode.h"
#include <string>

struct RenderCommand {
	std::string msg;
	RenderMode mode;

	RenderCommand(RenderMode mode, std::string msg = "") : mode(mode), msg(msg) {}
};