#include "FillingRenderAction.h"
#include "FillingRenderer.h"
#include "ShapeStore.h"
#include "WindowHandler.h"
#include <memory>

void FillingRenderAction::onMouseClick(Input::MouseInput mi)
{
	clickedPoints.push_back(mi.clickedPos);

	bool isCircleWithLines = command.msg == "Algo:CircleWithLines";
	bool isCircleWithCircles = command.msg == "Algo:CircleWithCircles";
	bool isHermite = command.msg == "Algo:HermiteCurves";
	bool isBezier = command.msg == "Algo:BezierCurves";
	bool isPolygon = command.msg == "Algo:PolygonFilling";
	bool isRecursiveFlood = command.msg == "Algo:RecursiveFloodFill";
	bool isNonRecursiveFlood = command.msg == "Algo:NonRecursiveFloodFill";

	auto handler = WindowHandler::getInstance();
	auto pipeline = RenderPipeline::getInstance();
	auto color = pipeline->getDrawingColor();

	std::unique_ptr<ShapeRenderer> shape = nullptr;

	// Circle filling algorithms need center and radius (2 clicks)
	if ((isCircleWithLines || isCircleWithCircles) && clickedPoints.size() == 2) {
		logger << "Drawing circle filling with " << command.msg << "\n";
		Point center = clickedPoints[0];
		Point edge = clickedPoints[1];
		int radius = center.distance(edge);

		int quadrant = command.msg.back() - '0'; // Extract quadrant from message (last character)

		FillingRenderer::FillingType type = isCircleWithLines 
			? FillingRenderer::FillingType::CircleWithLines 
			: FillingRenderer::FillingType::CircleWithCircles;

		shape = std::make_unique<FillingRenderer>(center, radius, quadrant, type, color);
		InvalidateRect(handler->getHWND(), nullptr, TRUE);
		ShapeStore::addShape(std::move(shape));
		isDone = true;
	}
	// Hermite and Bezier curves need a bounding box (2 clicks: top-left and bottom-right)
	else if ((isHermite || isBezier) && clickedPoints.size() == 2) {
		logger << "Drawing curve filling with " << command.msg << "\n";
		Point topLeft = clickedPoints[0];
		Point bottomRight = clickedPoints[1];

		FillingRenderer::FillingType type = isHermite 
			? FillingRenderer::FillingType::HermiteCurves 
			: FillingRenderer::FillingType::BezierCurves;

		shape = std::make_unique<FillingRenderer>(topLeft, bottomRight, type, color);
		InvalidateRect(handler->getHWND(), nullptr, TRUE);
		ShapeStore::addShape(std::move(shape));
		isDone = true;
	}
	// Polygon filling needs multiple points (need a special ending mechanism)
	else if (isPolygon) {
		// For simplicity, require 4 points (a quadrilateral)
		if (clickedPoints.size() < 4) {
			logger << "Clicked for polygon point " << clickedPoints.size() << "/4\n";
			return;
		}

		// Convert to Filling::Point array
		Filling::Point pts[4];
		for (int i = 0; i < 4; ++i) {
			pts[i] = Filling::Point(clickedPoints[i].x, clickedPoints[i].y);
		}

		logger << "Drawing polygon filling\n";
		shape = std::make_unique<FillingRenderer>(pts, 4, FillingRenderer::FillingType::ConvexPolygon, color);
		InvalidateRect(handler->getHWND(), nullptr, TRUE);
		ShapeStore::addShape(std::move(shape));
		isDone = true;
	}
	// Flood fill algorithms need only a starting point (1 click)
	else if ((isRecursiveFlood || isNonRecursiveFlood) && clickedPoints.size() == 1) {
		logger << "Drawing flood fill with " << command.msg << "\n";
		Point startPoint = clickedPoints[0];

		FillingRenderer::FillingType type = isRecursiveFlood 
			? FillingRenderer::FillingType::RecursiveFloodFill 
			: FillingRenderer::FillingType::NonRecursiveFloodFill;

		shape = std::make_unique<FillingRenderer>(startPoint, type, color);
		InvalidateRect(handler->getHWND(), nullptr, TRUE);
		ShapeStore::addShape(std::move(shape));
		isDone = true;
	}
	else if (clickedPoints.size() > (isPolygon ? 4 : 2)) {
		logger << "Too many clicks. Resetting.\n";
		clickedPoints.clear();
	}
}
