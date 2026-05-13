#pragma once

#include "AppContext.h"
#include <memory>
#include <optional>
#include <stdexcept>
#include "Windows.h"

#include "RenderCommand.h"

class RenderPipeline {

public:
	explicit RenderPipeline(AppContext& ctx) : m_Context (ctx) {
		m_Instance = this;
	}

	static RenderPipeline* getInstance() {
		if (m_Instance == nullptr) {
			throw std::runtime_error("Render Pipeline is not initialized");
		}

		return m_Instance;
	}

	void holdPipeline(RenderCommand cmdToHold) {
		heldCommand.emplace(cmdToHold);
	}

	void releasePipeline() {
		heldCommand.reset();
	}

	bool isHeld() const {
		return heldCommand.has_value();
	}

	std::optional<RenderCommand> getRenderCommand() const;

	COLORREF getDrawingColor() const {
		return drawingColor;
	}

	void setDrawingColor(COLORREF color) {
		drawingColor = color;
	}

private:
	static RenderPipeline* m_Instance;
	AppContext& m_Context;
	std::optional<RenderCommand> heldCommand;

	COLORREF drawingColor = RGB(200, 0, 0);
};