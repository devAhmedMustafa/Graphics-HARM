#pragma once 
#include <windows.h>


void DirectEllipse(HDC hdc, int xc, int yc, int rx, int ry, COLORREF color);

void PolarEllipse(HDC hdc, int xc, int yc, int rx, int ry, COLORREF color);

void MidpointEllipse(HDC hdc, int xc, int yc, int rx, int ry, COLORREF color);
void Draw4Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color);