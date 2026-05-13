#pragma once

#include "UtilTypes.h"
#include "Logger.h"
#include <stack>
#include <vector>
#include <functional>
#include <atomic>
#include <mutex>

class Input {

public:
	struct MouseInput {
		Point clickedPos;
		unsigned short button;
	};

	static void pushMouseClick(const MouseInput& input) {
		if (!capturing) {
			return;
		}

		for (const auto& callback : mouseClickCallbacks) {
			callback.second(input);
		}
		logger << "Mouse click at (" << input.clickedPos.x << ", " << input.clickedPos.y << ")\n";
	}

	static void startCaputring() {
		logger << "Started capturing input\n";
		capturing = true;
	}

	static void stopCapturing() {
		logger << "Stopped capturing input\n";
		capturing = false;
	}

	static size_t registerMouseClickCallback(const std::function<void(MouseInput)>& callback) {
		std::lock_guard lock(callbacksMutex);
		size_t id = nextId++;
		mouseClickCallbacks.emplace_back(id, callback);
		return id;
	}
	static void unregisterMouseClickCallback(size_t id) {
		std::lock_guard lock(callbacksMutex);
		mouseClickCallbacks.erase(
			std::remove_if(mouseClickCallbacks.begin(), mouseClickCallbacks.end(),
				[id](auto &p){ return p.first == id; }), mouseClickCallbacks.end());
	}
private:
	static Logger logger;
	static bool capturing;

	static std::vector<std::pair<size_t, std::function<void(MouseInput)>>> mouseClickCallbacks;
	static size_t nextId;
	static std::mutex callbacksMutex;
};