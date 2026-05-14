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

	// Parse color (always first token after type)
	COLORREF color = 0;
	if (!tokens.empty()) {
		color = static_cast<COLORREF>(std::stoul(tokens[0]));
	}

	// Recreate shape based on type
	if (shapeType == "DDALine" && tokens.size() >= 5) {
		Point start = { std::stoi(tokens[1]), std::stoi(tokens[2]) };
		Point end = { std::stoi(tokens[3]), std::stoi(tokens[4]) };
		return std::make_unique<DDALineRenderer>(start, end, color);
	}
	else if (shapeType == "MidpointLine" && tokens.size() >= 5) {
		Point start = { std::stoi(tokens[1]), std::stoi(tokens[2]) };
		Point end = { std::stoi(tokens[3]), std::stoi(tokens[4]) };
		return std::make_unique<MidpointLineRenderer>(start, end, color);
	}
	else if (shapeType == "ParametricLine" && tokens.size() >= 5) {
		Point start = { std::stoi(tokens[1]), std::stoi(tokens[2]) };
		Point end = { std::stoi(tokens[3]), std::stoi(tokens[4]) };
		return std::make_unique<ParametricLineRenderer>(start, end, color);
	}
	else if (shapeType == "DirectCircle" && tokens.size() >= 4) {
		Point center = { std::stoi(tokens[1]), std::stoi(tokens[2]) };
		int radius = std::stoi(tokens[3]);
		return std::make_unique<DirectCircleRenderer>(center, radius, color);
	}
	else if (shapeType == "MidPointCircle" && tokens.size() >= 4) {
		Point center = { std::stoi(tokens[1]), std::stoi(tokens[2]) };
		int radius = std::stoi(tokens[3]);
		return std::make_unique<MidPointCircleRenderer>(color, center, radius);
	}
	else if (shapeType == "PolarCircle" && tokens.size() >= 4) {
		Point center = { std::stoi(tokens[1]), std::stoi(tokens[2]) };
		int radius = std::stoi(tokens[3]);
		return std::make_unique<PolarCircleRenderer>(color, center, radius);
	}
	else if (shapeType == "IterPolarCircle" && tokens.size() >= 4) {
		Point center = { std::stoi(tokens[1]), std::stoi(tokens[2]) };
		int radius = std::stoi(tokens[3]);
		return std::make_unique<IterPolarCircleRenderer>(color, center, radius);
	}
	else if (shapeType == "ModMidCircle" && tokens.size() >= 4) {
		Point center = { std::stoi(tokens[1]), std::stoi(tokens[2]) };
		int radius = std::stoi(tokens[3]);
		return std::make_unique<ModMidCircleRenderer>(color, center, radius);
	}
	else if (shapeType == "DirectEllipse" && tokens.size() >= 5) {
		Point center = { std::stoi(tokens[1]), std::stoi(tokens[2]) };
		int rx = std::stoi(tokens[3]);
		int ry = std::stoi(tokens[4]);
		return std::make_unique<DirectEllipseRenderer>(center.x, center.y, rx, ry, color);
	}
	else if (shapeType == "MidpointEllipse" && tokens.size() >= 5) {
		Point center = { std::stoi(tokens[1]), std::stoi(tokens[2]) };
		int rx = std::stoi(tokens[3]);
		int ry = std::stoi(tokens[4]);
		return std::make_unique<MidpointEllipseRenderer>(center.x, center.y, rx, ry, color);
	}
	else if (shapeType == "PolarEllipse" && tokens.size() >= 5) {
		Point center = { std::stoi(tokens[1]), std::stoi(tokens[2]) };
		int rx = std::stoi(tokens[3]);
		int ry = std::stoi(tokens[4]);
		return std::make_unique<PolarEllipseRenderer>(center.x, center.y, rx, ry, color);
	}

	else if (shapeType == "Polygon" && tokens.size() >= 3) {
		size_t pointCount = std::stoul(tokens[1]);
		std::vector<Point> points;
		for (size_t i = 0; i < pointCount; ++i) {
			int x = std::stoi(tokens[2 + i * 2]);
			int y = std::stoi(tokens[3 + i * 2]);
			points.push_back({ x, y });
		}
		return std::make_unique<PolygonRenderer>(points, color);
	}
	else if (shapeType == "Point" && tokens.size() >= 3) {
		Point point = { std::stoi(tokens[1]), std::stoi(tokens[2]) };
		return std::make_unique<PointRenderer>(point, color);
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
