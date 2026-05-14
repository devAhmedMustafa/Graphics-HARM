#include "ShapeStore.h"
#include "DDALineRenderer.h"
#include "MidpointLineRenderer.h"
#include "ParametricLineRenderer.h"
#include "DirectCircleRenderer.h"
#include "MidPointCircleRenderer.h"
#include "PolarCircleRenderer.h"
#include "IterPolarCircleRenderer.h"
#include "ModMidCircleRenderer.h"
#include "DirectEllipseRenderer.h"
#include "MidpointEllipseRenderer.h"
#include "PolarEllipseRenderer.h"
#include "FillingRenderer.h"
#include "ClippingRenderer.h"
#include "CardinalCurveRenderer.h"
#include "PolygonRenderer.h"
#include "PointRenderer.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<std::unique_ptr<ShapeRenderer>> ShapeStore::shapes;
std::mutex ShapeStore::locker;

void ShapeStore::saveShapes(const std::string& filename)
{
	std::lock_guard<std::mutex> guard(locker);
	std::ofstream file(filename);
	
	if (!file.is_open()) {
		std::cerr << "Failed to open file for saving: " << filename << "\n";
		return;
	}

	// Write number of shapes
	file << shapes.size() << "\n";

	// Serialize each shape
	for (const auto& shape : shapes) {
		if (shape) {
			file << shape->getShapeType() << "|" << shape->serialize() << "\n";
		}
	}

	file.close();
	std::cout << "Shapes saved to " << filename << " (" << shapes.size() << " shapes)\n";
}

// Helper function to parse serialized shape data
static std::unique_ptr<ShapeRenderer> deserializeShape(const std::string& shapeType, const std::string& data)
{
	std::istringstream iss(data);
	std::string token;
	std::vector<std::string> tokens;

	while (std::getline(iss, token, ',')) {
		tokens.push_back(token);
	}

	bool seen = true;
	if (!tokens.empty()) {
		seen = static_cast<bool>(std::stoul(tokens[0]));
	}

	COLORREF color = 0;
	if (tokens.size() >= 2) {
		color = static_cast<COLORREF>(std::stoul(tokens[1]));
	}

	// Recreate shape based on type
	if (shapeType == "DDALine" && tokens.size() >= 6) {
		Point start = { std::stoi(tokens[2]), std::stoi(tokens[3]) };
		Point end = { std::stoi(tokens[4]), std::stoi(tokens[5]) };
		auto shape = std::make_unique<DDALineRenderer>(start, end, color);
		shape->seen = seen;
		return shape;
	}
	else if (shapeType == "MidpointLine" && tokens.size() >= 6) {
		Point start = { std::stoi(tokens[2]), std::stoi(tokens[3]) };
		Point end = { std::stoi(tokens[4]), std::stoi(tokens[5]) };
		auto shape = std::make_unique<MidpointLineRenderer>(start, end, color);
		shape->seen = seen;
		return shape;
	}
	else if (shapeType == "ParametricLine" && tokens.size() >= 6) {
		Point start = { std::stoi(tokens[2]), std::stoi(tokens[3]) };
		Point end = { std::stoi(tokens[4]), std::stoi(tokens[5]) };
		auto shape = std::make_unique<ParametricLineRenderer>(start, end, color);
		shape->seen = seen;
		return shape;
	}
	else if (shapeType == "DirectCircle" && tokens.size() >= 5) {
		Point center = { std::stoi(tokens[2]), std::stoi(tokens[3]) };
		int radius = std::stoi(tokens[4]);
		auto shape = std::make_unique<DirectCircleRenderer>(center, radius, color);
		shape->seen = seen;
		return shape;
	}
	else if (shapeType == "MidPointCircle" && tokens.size() >= 5) {
		Point center = { std::stoi(tokens[2]), std::stoi(tokens[3]) };
		int radius = std::stoi(tokens[4]);
		auto shape = std::make_unique<MidPointCircleRenderer>(color, center, radius);
		shape->seen = seen;
		return shape;
	}
	else if (shapeType == "PolarCircle" && tokens.size() >= 5) {
		Point center = { std::stoi(tokens[2]), std::stoi(tokens[3]) };
		int radius = std::stoi(tokens[4]);
		auto shape = std::make_unique<PolarCircleRenderer>(color, center, radius);
		shape->seen = seen;
		return shape;
	}
	else if (shapeType == "IterPolarCircle" && tokens.size() >= 5) {
		Point center = { std::stoi(tokens[2]), std::stoi(tokens[3]) };
		int radius = std::stoi(tokens[4]);
		auto shape = std::make_unique<IterPolarCircleRenderer>(color, center, radius);
		shape->seen = seen;
		return shape;
	}
	else if (shapeType == "ModMidCircle" && tokens.size() >= 5) {
		Point center = { std::stoi(tokens[2]), std::stoi(tokens[3]) };
		int radius = std::stoi(tokens[4]);
		auto shape = std::make_unique<ModMidCircleRenderer>(color, center, radius);
		shape->seen = seen;
		return shape;
	}
	else if (shapeType == "DirectEllipse" && tokens.size() >= 6) {
		Point center = { std::stoi(tokens[2]), std::stoi(tokens[3]) };
		int rx = std::stoi(tokens[4]);
		int ry = std::stoi(tokens[5]);
		auto shape = std::make_unique<DirectEllipseRenderer>(center.x, center.y, rx, ry, color);
		shape->seen = seen;
		return shape;
	}
	else if (shapeType == "MidpointEllipse" && tokens.size() >= 6) {
		Point center = { std::stoi(tokens[2]), std::stoi(tokens[3]) };
		int rx = std::stoi(tokens[4]);
		int ry = std::stoi(tokens[5]);
		auto shape = std::make_unique<MidpointEllipseRenderer>(center.x, center.y, rx, ry, color);
		shape->seen = seen;
		return shape;
	}
	else if (shapeType == "PolarEllipse" && tokens.size() >= 6) {
		Point center = { std::stoi(tokens[2]), std::stoi(tokens[3]) };
		int rx = std::stoi(tokens[4]);
		int ry = std::stoi(tokens[5]);
		auto shape = std::make_unique<PolarEllipseRenderer>(center.x, center.y, rx, ry, color);
		shape->seen = seen;
		return shape;
	}

	else if (shapeType == "Polygon" && tokens.size() >= 4) {
		size_t pointCount = std::stoul(tokens[2]);
		std::vector<Point> points;
		for (size_t i = 0; i < pointCount; ++i) {
			int x = std::stoi(tokens[3 + i * 2]);
			int y = std::stoi(tokens[4 + i * 2]);
			points.push_back({ x, y });
		}
		auto shape = std::make_unique<PolygonRenderer>(points, color);
		shape->seen = seen;
		return shape;
	}
	else if (shapeType == "Point" && tokens.size() >= 4) {
		Point point = { std::stoi(tokens[2]), std::stoi(tokens[3]) };
		auto shape = std::make_unique<PointRenderer>(point, color);
		shape->seen = seen;
		return shape;
	}
	else if (shapeType == "Clipping" && tokens.size() >= 9) {
		size_t shapeIndex = std::stoul(tokens[2]);
		Point clipStart = { std::stoi(tokens[3]), std::stoi(tokens[4]) };
		Point clipEnd = { std::stoi(tokens[5]), std::stoi(tokens[6]) };
		ClippingRenderer::ClippingType clipType = static_cast<ClippingRenderer::ClippingType>(std::stoul(tokens[7]));
		auto shape = std::make_unique<ClippingRenderer>(shapeIndex, clipStart, clipEnd, clipType, color);
		shape->seen = seen;
		return shape;
	}
	else if (shapeType == "CardinalCurve" && tokens.size() >= 10) {
		std::vector<Point> controlPoints;
		for (size_t i = 2; i + 1 < tokens.size(); i += 2) {
			int x = std::stoi(tokens[i]);
			int y = std::stoi(tokens[i + 1]);
			controlPoints.push_back({ x, y });
		}
		auto shape = std::make_unique<CardinalCurveRenderer>(color, controlPoints);
		shape->seen = seen;
		return shape;
	}

	return nullptr;
}

void ShapeStore::loadShapes(const std::string& filename)
{
	std::lock_guard<std::mutex> guard(locker);
	std::ifstream file(filename);

	if (!file.is_open()) {
		std::cerr << "Failed to open file for loading: " << filename << "\n";
		return;
	}

	shapes.clear();

	std::string line;
	size_t shapeCount = 0;

	// Read number of shapes
	if (std::getline(file, line)) {
		try {
			shapeCount = std::stoul(line);
		}
		catch (...) {
			std::cerr << "Invalid shape count in file\n";
			file.close();
			return;
		}
	}

	// Load each shape
	size_t loadedCount = 0;
	while (std::getline(file, line) && loadedCount < shapeCount) {
		if (line.empty()) continue;

		size_t pipePos = line.find('|');
		if (pipePos == std::string::npos) continue;

		std::string shapeType = line.substr(0, pipePos);
		std::string shapeData = line.substr(pipePos + 1);

		auto shape = deserializeShape(shapeType, shapeData);
		if (shape) {
			shapes.push_back(std::move(shape));
			loadedCount++;
		}
	}

	file.close();

	auto handler = WindowHandler::getInstance();
	if (handler) {
		InvalidateRect(handler->getHWND(), nullptr, TRUE);
	}

	std::cout << "Loaded " << loadedCount << " shapes from " << filename << "\n";
}
