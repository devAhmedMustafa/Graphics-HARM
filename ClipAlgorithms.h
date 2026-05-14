#pragma once

#include <windows.h>
#include <vector>

// Helper to draw clipping region border
void DrawClipBorder(HDC hdc, int bx1, int by1, int bx2, int by2,
    COLORREF col = RGB(180, 0, 0));

// Cohen-Sutherland clipping algorithm constants and utilities
namespace CS {
    constexpr int INSIDE = 0;
    constexpr int LEFT = 1;
    constexpr int RIGHT = 2;
    constexpr int TOP = 4;
    constexpr int BOTTOM = 8;

    int regionCode(int x, int y, int bx1, int by1, int bx2, int by2);
}

// Point clipping functions
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

// Line clipping functions
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

// Polygon clipping functions (Sutherland-Hodgman algorithm)
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


//void ClipAsCircle(HDC hdc,
//    int x, int y,
//    int cx, int cy, int r,
//    COLORREF col = RGB(0, 0, 0),
//    bool drawBorder = true)
//{
//    if (drawBorder)
//    {
//        // Draw the circular clip window (dashed appearance via arc segments)
//        HPEN pen = CreatePen(PS_DASH, 1, RGB(180, 0, 0));
//        HPEN old = (HPEN)SelectObject(hdc, pen);
//        Arc(hdc, cx - r, cy - r, cx + r, cy + r, cx + r, cy, cx + r, cy); // full ellipse
//        SelectObject(hdc, old);
//        DeleteObject(pen);
//    }
//
//    float dx = (float)(x - cx), dy = (float)(y - cy);
//    if (dx * dx + dy * dy <= (float)(r * r))
//        SetPixel(hdc, x, y, col);
//}
//
//
//void ClipAsCircle(HDC hdc,
//    int x1, int y1, int x2, int y2,
//    int cx, int cy, int r,
//    COLORREF col = RGB(0, 0, 0),
//    bool drawBorder = true)
//{
//    if (drawBorder)
//    {
//        HPEN pen = CreatePen(PS_DASH, 1, RGB(180, 0, 0));
//        HPEN old = (HPEN)SelectObject(hdc, pen);
//        Arc(hdc, cx - r, cy - r, cx + r, cy + r, cx + r, cy, cx + r, cy);
//        SelectObject(hdc, old);
//        DeleteObject(pen);
//    }
//
//    // Direction vector and offset from circle centre
//    float dx = (float)(x2 - x1), dy = (float)(y2 - y1);
//    float fx = (float)(x1 - cx), fy = (float)(y1 - cy);
//    float R = (float)r;
//
//    // Quadratic coefficients:  a·t² + b·t + c = 0
//    float a = dx * dx + dy * dy;
//    float b = 2.f * (fx * dx + fy * dy);
//    float c = fx * fx + fy * fy - R * R;
//    float det = b * b - 4.f * a * c;
//
//    if (a < 1e-6f || det < 0.f) return;  // parallel degenerate or no intersection
//
//    float sqrtDet = std::sqrt(det);
//    float t0 = (-b - sqrtDet) / (2.f * a);   // entry parameter
//    float t1 = (-b + sqrtDet) / (2.f * a);   // exit  parameter
//
//    // Clamp to the segment [0,1]
//    t0 = max(0.f, min(1.f, t0));
//    t1 = max(0.f, min(1.f, t1));
//
//    if (t0 >= t1) return;   // visible portion has zero length
//
//    // Compute the clipped endpoints
//    int px1 = x1 + (int)(t0 * dx), py1 = y1 + (int)(t0 * dy);
//    int px2 = x1 + (int)(t1 * dx), py2 = y1 + (int)(t1 * dy);
//
//    HPEN pen = CreatePen(PS_SOLID, 1, col);
//    HPEN old = (HPEN)SelectObject(hdc, pen);
//    MoveToEx(hdc, px1, py1, NULL);
//    LineTo(hdc, px2, py2);
//    SelectObject(hdc, old);
//    DeleteObject(pen);
//}