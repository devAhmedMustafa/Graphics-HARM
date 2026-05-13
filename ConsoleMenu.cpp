#include "ConsoleMenu.h"
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
}

void ConsoleMenu::PreferencesMenu() {
	std::cout << "1) Change background color\n"
		<< "2) Change cursor\n"
		<< "3) Change drawing color\n"
		;
}

void ConsoleMenu::LinesMenu() {
	std::cout << "1) DDA\n"
		<< "2) Midpoint\n"
		<< "3) Parametric\n"
		;
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
}

void ConsoleMenu::CurvesMenu() {
}

void ConsoleMenu::FillingMenu() {
	std::cout << "1) Fill circle with lines\n"
		<< "2) Fill circle with circles\n"
		<< "3) Fill square with hermit curves\n"
		<< "4) Fill rectangle with bezier curves\n"
		<< "5) Convext and non-convex filling\n"
		<< "6) Recursive flood fill\n"
		<< "7) Non-recursive flood fill\n"
		;
}

void ConsoleMenu::ClippingMenu() {
	std::cout << "1) Clip with a rectangle\n"
		<< "2) Clip with a square\n"
		<< "3) Line clipping with Liang-Barsky\n"
		<< "4) Polygon clipping with Sutherland-Hodgman\n"
		<< "5) Polygon clipping with Weiler-Atherton\n"
		;
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