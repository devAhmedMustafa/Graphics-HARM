#pragma once

#include "AppContext.h"

class ConsoleMenu {

public:
	ConsoleMenu(AppContext& ctx) : m_Context(ctx) {}

	void run();

private:
	AppContext& m_Context;

	void FileMenu();
	void PreferencesMenu();
	void LinesMenu();
	void CirclesMenu();
	void EllipseMenu();
	void CurvesMenu();
	void FillingMenu();
	void PointsMenu();
	void ClippingMenu();

};