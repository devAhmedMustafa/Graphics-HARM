#pragma once 
#include <windows.h>

void DirectCircle(HDC hdc, int xc, int yc, int r, COLORREF color);
void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color);

void ModifiedMidpointCircle(HDC hdc, int xc, int yc, int r, COLORREF color);
void PolarCircle(HDC hdc, int xc, int yc, int r, COLORREF color);
void IterativePolarCircle(HDC hdc, int xc, int yc, int r, COLORREF color);
