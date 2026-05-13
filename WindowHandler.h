#pragma once
#include <Windows.h>
#include <windowsx.h>
#include "Logger.h"
#include "RenderAction.h"
#include "CircleAlgorithms.h"

class WindowHandler
{

public:

    static WindowHandler* getInstance() {
        if (instance == nullptr) {
			throw std::runtime_error("WindowHandler instance not initialized. Call SetupWindow first.");
        }
        return instance;
    }

    WindowHandler() {
		instance = this;
    }

    HWND SetupWindow(HINSTANCE instance, int nsh);

    HWND getHWND() const {
        return hwnd;
    }

private:
    static WindowHandler* instance;
    Logger logger;
    HWND hwnd;
    RenderAction* currentAction = nullptr;

    static LRESULT WINAPI WndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp);
};