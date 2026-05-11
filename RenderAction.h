#pragma once
#include <Windows.h>
#include "RenderCommand.h"

class RenderAction {

public:
	RenderAction(const RenderCommand& command) : command(command) {}
	virtual LRESULT draw(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp) = 0;

private:
	RenderCommand command;
};