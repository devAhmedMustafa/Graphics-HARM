#pragma once

#include <vector>
#include <mutex>
#include <memory>
#include "ShapeRenderer.h"

class ShapeStore
{
public:

	static void addShape(std::unique_ptr<ShapeRenderer> shape) {
		std::lock_guard<std::mutex> guard(locker);
		shapes.push_back(std::move(shape));
	}

	static std::vector<ShapeRenderer*> getShapes() {
		std::lock_guard<std::mutex> guard(locker);
		std::vector<ShapeRenderer*> rawPtrs;
		for (auto& shape : shapes) {
			rawPtrs.push_back(shape.get());
		}
		return rawPtrs;
	}

private:
	static std::vector<std::unique_ptr<ShapeRenderer>> shapes;
	static std::mutex locker;
};

