#include "Windows.h"
#include <windowsx.h>
#include <cstdio>
#include <thread>

#include "WindowHandler.h"
#include "AppContext.h"
#include "ConsoleMenu.h"
#include "RenderPipeline.h"

int main(int argc, char* argv[])
{
	HINSTANCE h = GetModuleHandle(NULL);
	int nsh = SW_SHOW;
	AppContext app;
	ConsoleMenu menu(app);
	RenderPipeline rePipe(app);

	AllocConsole();

	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONIN$", "r", stdin);

	HWND hwnd = SetupWindow(h, nsh);

	MSG msg;

	app.running = true;

	std::thread consoleThread([&menu]() {menu.run(); });

	while (app.running)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				app.running = false;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	consoleThread.join();

	return 0;
}