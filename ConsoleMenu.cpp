#include "ConsoleMenu.h"
#include "ShapeStore.h"
#include <iostream>
#include <string>
#include <limits>
#include <windows.h>

static void BringConsoleToFront()
{
	// Ensure console has focus (helps when another window was created and stole focus)
	HWND hwnd = GetConsoleWindow();
	if (hwnd) ::SetForegroundWindow(hwnd);
}

void ConsoleMenu::FileMenu() {
	std::cout << "1) Clear screen\n"
		<< "2) Save\n"
		<< "3) Load\n"
		;

	int action;
	std::cin >> action;
	std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

	switch (action)
	{
	case 1:
		ShapeStore::clearShapes();
		break;

	case 2: {

		std::cout << "Enter filename to save: ";
		std::string saveFilename;
		std::getline(std::cin, saveFilename);
		ShapeStore::saveShapes(saveFilename);
		break;	
	}

	case 3: {
		std::cout << "Enter filename to load: ";
		std::string loadFilename;
		std::getline(std::cin, loadFilename);
		ShapeStore::loadShapes(loadFilename);
		break;
	}

	default:
		std::cout << "Not implemented yet\n";
		break;
	}
}

void ConsoleMenu::PreferencesMenu() {
	std::cout << "1) Change background color\n"
		<< "2) Change cursor\n"
		<< "3) Change drawing color\n"
		;

	int action;
	std::cin >> action;
	std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

	switch (action)
	{
	case 1:
		m_Context.pushToChannel(RenderCommand(RenderMode::ChangeBgToWhite));
		break;
	case 2:
		m_Context.pushToChannel(RenderCommand(RenderMode::ChangeCursor));
		break;
	case 3:
	{
		std::string color;
		std::cout << "Enter drawing color (e.g., Red Green Blue): ";
		std::getline(std::cin, color);

		m_Context.pushToChannel(RenderCommand(RenderMode::SetDrawingColor, color));
		break;
	}
	default:
		std::cout << "Not implemented yet\n";
		break;
	}
}

void ConsoleMenu::LinesMenu() {
	std::cout << "1) DDA\n"
		<< "2) Midpoint\n"
		<< "3) Parametric\n"
		;

	int action;
	std::cin >> action;

	std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	switch (action)
	{
	case 1:
		m_Context.pushToChannel(RenderCommand(RenderMode::DrawLine, "Algo:DDA"));
		break;
	case 2:
		m_Context.pushToChannel(RenderCommand(RenderMode::DrawLine, "Algo:Midpoint"));
		break;
	case 3:
		m_Context.pushToChannel(RenderCommand(RenderMode::DrawLine, "Algo:Parametric"));
		break;
	default:
		std::cout << "Not implemented yet\n";
		break;
	}
}

void ConsoleMenu::CirclesMenu() {
	std::cout << "1) Direct\n"
		<< "2) Polar\n"
		<< "3) Iterative Polar\n"
		<< "4) Midpoint\n"
		<< "5) Modified Midpoint\n"
		;

	int action;
	std::cin >> action;
	std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

	switch (action)
	{
	case 1:
		m_Context.pushToChannel(RenderCommand(RenderMode::DrawCircle, "Algo:Direct"));
		break;
	case 2:
		m_Context.pushToChannel(RenderCommand(RenderMode::DrawCircle, "Algo:Polar"));
		break;
	case 3:
		m_Context.pushToChannel(RenderCommand(RenderMode::DrawCircle, "Algo:IterativePolar"));
		break;
	case 4:
		m_Context.pushToChannel(RenderCommand(RenderMode::DrawCircle, "Algo:Midpoint"));
		break;
	case 5:
		m_Context.pushToChannel(RenderCommand(RenderMode::DrawCircle, "Algo:ModifiedMidpoint"));
		break;
	default:
		std::cout << "Not implemented yet\n";
		break;
	}
}

void ConsoleMenu::EllipseMenu() {
	std::cout << "1) Direct\n"
		<< "2) Polar\n"
		<< "3) Midpoint\n"
		;

	int action;
	std::cin >> action;
	std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

	switch (action)
	{
	case 1:
		m_Context.pushToChannel(RenderCommand(RenderMode::DrawEllipse, "Algo:Direct"));
		break;
	case 2:
		m_Context.pushToChannel(RenderCommand(RenderMode::DrawEllipse, "Algo:Polar"));
		break;
	case 3:
		m_Context.pushToChannel(RenderCommand(RenderMode::DrawEllipse, "Algo:Midpoint"));
		break;
	default:
		std::cout << "Not implemented yet\n";
		break;
	}
}

void ConsoleMenu::CurvesMenu() {
	m_Context.pushToChannel(RenderCommand(RenderMode::DrawCurve));
}

void ConsoleMenu::FillingMenu() {
	std::cout << "1) Fill circle with lines\n"
		<< "2) Fill circle with circles\n"
		<< "3) Fill square with hermit curves\n"
		<< "4) Fill rectangle with bezier curves\n"
		<< "5) Convex and non-convex filling\n"
		<< "6) Recursive flood fill\n"
		<< "7) Non-recursive flood fill\n"
		;

	int action;
	std::cin >> action;
	std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

	switch (action)
	{
	case 1: {
		int quadrant;
		std::cout << "Select quadrant to fill:\n"
			<< " 1) Top-Right\n"
			<< " 2) Top-Left\n"
			<< " 3) Bottom-Left\n"
			<< " 4) Bottom-Right\n";
		std::cin >> quadrant;
		std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

		m_Context.pushToChannel(RenderCommand(RenderMode::Fill, "Algo:CircleWithLines-" + std::to_string(quadrant)));
		break;
	}
	case 2: {
		int quadrant;
		std::cout << "Select quadrant to fill:\n"
			<< " 1) Top-Right\n"
			<< " 2) Top-Left\n"
			<< " 3) Bottom-Left\n"
			<< " 4) Bottom-Right\n";
		std::cin >> quadrant;
		std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');


		m_Context.pushToChannel(RenderCommand(RenderMode::Fill, "Algo:CircleWithCircles-" + std::to_string(quadrant)));
		break;
	}
	case 3:
		m_Context.pushToChannel(RenderCommand(RenderMode::Fill, "Algo:HermiteCurves"));
		break;
	case 4:
		m_Context.pushToChannel(RenderCommand(RenderMode::Fill, "Algo:BezierCurves"));
		break;
	case 5:
		m_Context.pushToChannel(RenderCommand(RenderMode::Fill, "Algo:PolygonFilling"));
		break;
	case 6:
		m_Context.pushToChannel(RenderCommand(RenderMode::Fill, "Algo:RecursiveFloodFill"));
		break;
	case 7:
		m_Context.pushToChannel(RenderCommand(RenderMode::Fill, "Algo:NonRecursiveFloodFill"));
		break;
	default:
		std::cout << "Not implemented yet\n";
		break;
	}
}

void ConsoleMenu::ClippingMenu() {
	std::cout << "Available shapes to clip:\n";
	
	auto shapes = ShapeStore::getShapes();
	if (shapes.empty()) {
		std::cout << "No shapes available. Please draw a shape first.\n";
		return;
	}

	for (size_t i = 0; i < shapes.size(); ++i) {
		std::cout << " " << (i + 1) << ") Shape " << i << "\n";
	}

	std::cout << "\nSelect a shape to clip (1-" << shapes.size() << "): ";
	int shapeChoice;
	std::cin >> shapeChoice;
	std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

	if (shapeChoice < 1 || shapeChoice > (int)shapes.size()) {
		std::cout << "Invalid shape selection.\n";
		return;
	}

	std::cout << "\nClipping options:\n"
		<< "Rectangle Clipping:\n"
		<< "  1) Rectangle + Point\n"
		<< "  2) Rectangle + Line\n"
		<< "  3) Rectangle + Polygon\n"
		<< "Square Clipping:\n"
		<< "  4) Square + Point\n"
		<< "  5) Square + Line\n"
		;

	int action;
	std::cin >> action;
	std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

	// Store shape index in the message (shapeIndex:ClippingType)
	std::string shapeIndexMsg = std::to_string(shapeChoice - 1) + ":";

	switch (action)
	{
	case 1:
		m_Context.pushToChannel(RenderCommand(RenderMode::Clip, shapeIndexMsg + "RectClip_Point"));
		break;
	case 2:
		m_Context.pushToChannel(RenderCommand(RenderMode::Clip, shapeIndexMsg + "RectClip_Line"));
		break;
	case 3:
		m_Context.pushToChannel(RenderCommand(RenderMode::Clip, shapeIndexMsg + "RectClip_Polygon"));
		break;
	case 4:
		m_Context.pushToChannel(RenderCommand(RenderMode::Clip, shapeIndexMsg + "SquareClip_Point"));
		break;
	case 5:
		m_Context.pushToChannel(RenderCommand(RenderMode::Clip, shapeIndexMsg + "SquareClip_Line"));
		break;
	default:
		std::cout << "Not implemented yet\n";
		break;
	}
}

void ConsoleMenu::run() {
	BringConsoleToFront();

	std::cout << "Hello mr artist" << std::endl;

	while (true)
	{
		std::cout << "\nWhat you wanna open?\n"
			<< " 1) File Menu\n"
			<< " 2) Prefernces Menu\n"
			<< " 3) Lines Menu\n"
			<< " 4) Circles Menu\n"
			<< " 5) Ellipse Menu\n"
			<< " 6) Curves Menu\n"
			<< " 7) Filling Menu\n"
			<< " 8) Clipping Menu\n"
			<< " 0) Exit\n"
			<< std::endl;

		std::cout << "Your choice: ";
		std::cout.flush();

		std::string line;
		if (!std::getline(std::cin, line)) {
			std::cin.clear();
			continue;
		}

		int action = -1;
		try {
			action = std::stoi(line);
		}
		catch (...) {
			std::cout << "Invalid input. Please enter a number.\n";
			continue;
		}

		if (action == 0) break;

		switch (action)
		{
		case 1:
			FileMenu();
			break;
		case 2:
			PreferencesMenu();
			break;
		case 3:
			LinesMenu();
			break;
		case 4:
			CirclesMenu();
			break;

		case 5:
			EllipseMenu();
			break;

		case 6:
			CurvesMenu();
			break;

		case 7:
			FillingMenu();
			break;

		case 8:
			ClippingMenu();
			break;

		default:
			std::cout << "Not implemented yet\n";
			break;
		}

		std::cout << "\nPress ENTER to return to the main menu..." << std::endl;
		std::getline(std::cin, line);
	}
}