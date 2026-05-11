#pragma once

#include "AppContext.h"

class ConsoleMenu {

public:
	ConsoleMenu(AppContext& ctx) : m_Context(ctx) {}

	void run();

private:
	AppContext& m_Context;

};