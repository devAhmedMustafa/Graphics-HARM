#pragma once
#include <Windows.h>
#include "RenderPipeline.h"
#include "RenderAction.h"

LRESULT WINAPI WndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
{
    switch (mcode)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    // Render pipeline gets all remaining messages

    auto renderPipeline = RenderPipeline::getInstance();

    auto command = renderPipeline->getRenderCommand();
    if (!command.has_value())
        return DefWindowProc(hwnd, mcode, wp, lp);

    RenderAction* action = nullptr;
    switch (command->mode)
    {
    case RenderMode::ClearScreen:
        break;
    default:
        break;
    }

    if (!action)
        return DefWindowProc(hwnd, mcode, wp, lp);

    return action->draw(hwnd, mcode, wp, lp);
}

HWND SetupWindow(HINSTANCE instance, int nsh) {
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hInstance = instance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"renderer";
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wc);

	HWND hwnd = CreateWindow(L"renderer", L"Graphics Engine", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, instance, 0);

	ShowWindow(hwnd, nsh);
	UpdateWindow(hwnd);

	return hwnd;
}