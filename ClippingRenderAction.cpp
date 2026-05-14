#include "ClippingRenderAction.h"
#include "ClippingRenderer.h"
#include "ShapeStore.h"
#include "WindowHandler.h"
#include <memory>
#include <iostream>
#include <sstream>

void ClippingRenderAction::onMouseClick(Input::MouseInput mi)
{
	clickedPoints.push_back(mi.clickedPos);

	auto handler = WindowHandler::getInstance();
	auto pipeline = RenderPipeline::getInstance();
	auto color = GetBkColor(GetDC(handler->getHWND()));

	// Parse the command message to extract shape index and clipping type
	// Format: "shapeIndex:ClippingType" (e.g., "0:RectClip_Line")
	std::string msg = command.msg;
	size_t colonPos = msg.find(':');
	if (colonPos == std::string::npos) {
		logger << "Invalid clipping command format\n";
		isDone = true;
		return;
	}

	size_t shapeIndex = 0;
	try {
		shapeIndex = std::stoi(msg.substr(0, colonPos));
	}
	catch (...) {
		logger << "Failed to parse shape index from command\n";
		isDone = true;
		return;
	}

	std::string typeStr = msg.substr(colonPos + 1);

	// All clipping operations need 2 clicks (to define the clipping region)
	if (clickedPoints.size() == 2) {
		logger << "Applying clipping with " << typeStr << " to shape " << shapeIndex << "\n";
		
		// Get the shapes
		auto shapes = ShapeStore::getShapes();
		if (shapeIndex >= shapes.size()) {
			logger << "Invalid shape index: " << shapeIndex << "\n";
			isDone = true;
			return;
		}

		Point clipStart = clickedPoints[0];
		Point clipEnd = clickedPoints[1];

		ClippingRenderer::ClippingType clipType;
		
		// Map string to ClippingType
		if (typeStr == "RectClip_Point") {
			clipType = ClippingRenderer::ClippingType::RectClip_Point;
		}
		else if (typeStr == "RectClip_Line") {
			clipType = ClippingRenderer::ClippingType::RectClip_Line;
		}
		else if (typeStr == "RectClip_Polygon") {
			clipType = ClippingRenderer::ClippingType::RectClip_Polygon;
		}
		else if (typeStr == "SquareClip_Point") {
			clipType = ClippingRenderer::ClippingType::SquareClip_Point;
		}
		else if (typeStr == "SquareClip_Line") {
			clipType = ClippingRenderer::ClippingType::SquareClip_Line;
		}
		else {
			logger << "Unknown clipping type: " << typeStr << "\n";
			isDone = true;
			return;
		}

		logger << "Clipping region: (" << clipStart.x << "," << clipStart.y << ") to ("
			   << clipEnd.x << "," << clipEnd.y << ")\n";

		// Create and store the clipping renderer
		std::unique_ptr<ShapeRenderer> clippingShape = 
			std::make_unique<ClippingRenderer>(shapeIndex, clipStart, clipEnd, clipType, color);
		
		ShapeStore::addShape(std::move(clippingShape));
		InvalidateRect(handler->getHWND(), nullptr, TRUE);

		isDone = true;
	}
	else if (clickedPoints.size() > 2) {
		logger << "Too many clicks. Resetting.\n";
		clickedPoints.clear();
	}
}
