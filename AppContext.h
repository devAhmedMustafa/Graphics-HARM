#pragma once

#include <string>
#include <queue>
#include "RenderCommand.h"

class AppContext {

public:
	void pushToChannel(const RenderCommand& command) {
		channel.push(command);
	}

	void stop() {
		running = false;
	}

	RenderCommand getCommand() {
		auto command = channel.front();
		channel.pop();
		return command;
	}

	bool isRunning() const {
		return running;
	}

	friend class RenderPipeline;

private:
	std::queue<RenderCommand> channel;
	bool running = true;
};