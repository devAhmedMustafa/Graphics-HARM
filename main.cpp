#include "Windows.h"
#include <windowsx.h>
#include <cstdio>
#include <thread>

#include "WindowHandler.h"
#include "AppContext.h"
#include "ConsoleMenu.h"
#include "RenderPipeline.h"

#include "Logger.h"

int main(int argc, char* argv[])
{
	Logger logger;
	logger << "Starting application...\n";

	HINSTANCE h = GetModuleHandle(NULL);
	int nsh = SW_SHOW;
	AppContext app;
	ConsoleMenu menu(app);
	RenderPipeline rePipe(app);
	WindowHandler wh;

	AllocConsole();

	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONIN$", "r", stdin);

	HWND hwnd = wh.SetupWindow(h, nsh);

	MSG msg;

	std::thread consoleThread([&menu]() {menu.run(); });

	while (app.isRunning())
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				app.stop();
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	consoleThread.join();

	return 0;
}