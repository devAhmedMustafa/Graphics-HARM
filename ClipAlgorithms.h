#pragma once

#include <windows.h>
#include <vector>

void DrawClipBorder(HDC hdc, int bx1, int by1, int bx2, int by2,
    COLORREF col = RGB(180, 0, 0));

namespace CS {
    constexpr int INSIDE = 0;
    constexpr int LEFT = 1;
    constexpr int RIGHT = 2;
    constexpr int TOP = 4;
    constexpr int BOTTOM = 8;

    int regionCode(int x, int y, int bx1, int by1, int bx2, int by2);
}

void ClipPointAsRect(HDC hdc,
    int x, int y,
    int bx1, int by1, int bx2, int by2,
    COLORREF col = RGB(0, 0, 0),
    bool drawBorder = true);

void ClipPointAsSquare(HDC hdc,
    int x, int y,
    int bx1, int by1, int side,
    COLORREF col = RGB(0, 0, 0),
    bool drawBorder = true);

void ClipLineAsRect(HDC hdc,
    int x1, int y1, int x2, int y2,
    int bx1, int by1, int bx2, int by2,
    COLORREF col = RGB(0, 0, 0),
    bool drawBorder = true);

void ClipLineAsSquare(HDC hdc,
    int x1, int y1, int x2, int y2,
    int bx1, int by1, int side,
    COLORREF col = RGB(0, 0, 0),
    bool drawBorder = true);

void ClipPolygonAsRect(HDC hdc,
    int xs[], int ys[], int n,
    int bx1, int by1, int bx2, int by2,
    COLORREF col = RGB(0, 0, 0),
    bool drawBorder = true);

void ClipPolygonAsSquare(HDC hdc,
    int xs[], int ys[], int n,
    int bx1, int by1, int side,
    COLORREF col = RGB(0, 0, 0),
    bool drawBorder = true);