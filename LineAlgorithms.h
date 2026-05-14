#pragma once 
#include <windows.h>

void lineDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF C);
void MidPointLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c);
void ParametricLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c);
