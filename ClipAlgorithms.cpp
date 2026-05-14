#include "ClipAlgorithms.h"
#include <cmath>
#include <algorithm>

// Helper to draw clipping region border
void DrawClipBorder(HDC hdc, int bx1, int by1, int bx2, int by2, COLORREF col)
{
    HPEN pen = CreatePen(PS_DASH, 1, col);
    HPEN old = (HPEN)SelectObject(hdc, pen);
    MoveToEx(hdc, bx1, by1, NULL);
    LineTo(hdc, bx2, by1);
    LineTo(hdc, bx2, by2);
    LineTo(hdc, bx1, by2);
    LineTo(hdc, bx1, by1);
    SelectObject(hdc, old);
    DeleteObject(pen);
}

// Cohen-Sutherland region code calculation
int CS::regionCode(int x, int y, int bx1, int by1, int bx2, int by2)
{
    int code = INSIDE;
    if (x < bx1) code |= LEFT;
    else if (x > bx2) code |= RIGHT;
    if (y < by1) code |= TOP;
    else if (y > by2) code |= BOTTOM;
    return code;
}

// Point clipping against rectangle
void ClipPointAsRect(HDC hdc,
    int x, int y,
    int bx1, int by1, int bx2, int by2,
    COLORREF col,
    bool drawBorder)
{
    if (drawBorder) DrawClipBorder(hdc, bx1, by1, bx2, by2);

    if (x >= bx1 && x <= bx2 && y >= by1 && y <= by2)
        SetPixel(hdc, x, y, col);
}

// Line clipping against rectangle (Cohen-Sutherland algorithm)
void ClipLineAsRect(HDC hdc,
    int x1, int y1, int x2, int y2,
    int bx1, int by1, int bx2, int by2,
    COLORREF col,
    bool drawBorder)
{
    if (drawBorder) DrawClipBorder(hdc, bx1, by1, bx2, by2);

    float fx1 = (float)x1, fy1 = (float)y1;
    float fx2 = (float)x2, fy2 = (float)y2;

    int code1 = CS::regionCode((int)fx1, (int)fy1, bx1, by1, bx2, by2);
    int code2 = CS::regionCode((int)fx2, (int)fy2, bx1, by1, bx2, by2);

    while (true)
    {
        if (!(code1 | code2))
        {
            HPEN pen = CreatePen(PS_SOLID, 1, col);
            HPEN old = (HPEN)SelectObject(hdc, pen);
            MoveToEx(hdc, (int)fx1, (int)fy1, NULL);
            LineTo(hdc, (int)fx2, (int)fy2);
            SelectObject(hdc, old);
            DeleteObject(pen);
            return;
        }
        else if (code1 & code2)
        {
            return;
        }
        else
        {
            int codeOut = code1 ? code1 : code2;
            float x = 0, y = 0;

            if (codeOut & CS::BOTTOM)
            {
                x = fx1 + (fx2 - fx1) * ((float)by2 - fy1) / (fy2 - fy1);
                y = (float)by2;
            }
            else if (codeOut & CS::TOP)
            {
                x = fx1 + (fx2 - fx1) * ((float)by1 - fy1) / (fy2 - fy1);
                y = (float)by1;
            }
            else if (codeOut & CS::RIGHT)
            {
                y = fy1 + (fy2 - fy1) * ((float)bx2 - fx1) / (fx2 - fx1);
                x = (float)bx2;
            }
            else
            {
                y = fy1 + (fy2 - fy1) * ((float)bx1 - fx1) / (fx2 - fx1);
                x = (float)bx1;
            }

            if (codeOut == code1)
            {
                fx1 = x;  fy1 = y;
                code1 = CS::regionCode((int)fx1, (int)fy1, bx1, by1, bx2, by2);
            }
            else
            {
                fx2 = x;  fy2 = y;
                code2 = CS::regionCode((int)fx2, (int)fy2, bx1, by1, bx2, by2);
            }
        }
    }
}

// Helper for Sutherland-Hodgman polygon clipping
static std::vector<POINT> _SHClipEdge(
    const std::vector<POINT>& poly,
    int edgeType, int bx1, int by1, int bx2, int by2)
{
    std::vector<POINT> out;
    int n = (int)poly.size();
    if (n == 0) return out;

    auto inside = [&](POINT p) -> bool {
        switch (edgeType) {
        case 0: return p.x >= bx1;   // left
        case 1: return p.x <= bx2;   // right
        case 2: return p.y >= by1;   // top
        case 3: return p.y <= by2;   // bottom
        }
        return false;
        };

    // Parametric intersection of segment AB with this edge
    auto intersect = [&](POINT A, POINT B) -> POINT {
        float dx = (float)(B.x - A.x);
        float dy = (float)(B.y - A.y);
        float t = 0.f;
        switch (edgeType) {
        case 0: t = ((float)bx1 - A.x) / dx; break;
        case 1: t = ((float)bx2 - A.x) / dx; break;
        case 2: t = ((float)by1 - A.y) / dy; break;
        case 3: t = ((float)by2 - A.y) / dy; break;
        }
        return { (LONG)(A.x + t * dx), (LONG)(A.y + t * dy) };
        };

    for (int i = 0; i < n; i++)
    {
        POINT curr = poly[i];
        POINT prev = poly[(i - 1 + n) % n];

        bool currIn = inside(curr);
        bool prevIn = inside(prev);

        if (currIn)
        {
            if (!prevIn) out.push_back(intersect(prev, curr)); // entering
            out.push_back(curr);
        }
        else if (prevIn)
        {
            out.push_back(intersect(prev, curr));              // exiting
        }
    }
    return out;
}

// Polygon clipping against rectangle (Sutherland-Hodgman algorithm)
void ClipPolygonAsRect(HDC hdc,
    int xs[], int ys[], int n,
    int bx1, int by1, int bx2, int by2,
    COLORREF col,
    bool drawBorder)
{
    if (drawBorder) DrawClipBorder(hdc, bx1, by1, bx2, by2);
    if (n < 2) return;

    // Build initial polygon
    std::vector<POINT> poly(n);
    for (int i = 0; i < n; i++) { poly[i] = { xs[i], ys[i] }; }

    // Clip against all four edges in sequence (Sutherland-Hodgman)
    for (int edge = 0; edge < 4 && !poly.empty(); edge++)
        poly = _SHClipEdge(poly, edge, bx1, by1, bx2, by2);

    if (poly.size() < 2) return;

    // Draw the clipped polygon (closed outline)
    std::vector<POINT> pts(poly.begin(), poly.end());
    pts.push_back(pts.front());                 // close it

    HPEN pen = CreatePen(PS_SOLID, 1, col);
    HPEN old = (HPEN)SelectObject(hdc, pen);
    Polyline(hdc, pts.data(), (int)pts.size());
    SelectObject(hdc, old);
    DeleteObject(pen);
}

// Point clipping against square
void ClipPointAsSquare(HDC hdc,
    int x, int y,
    int bx1, int by1, int side,
    COLORREF col,
    bool drawBorder)
{
    ClipPointAsRect(hdc, x, y,
        bx1, by1, bx1 + side, by1 + side,
        col, drawBorder);
}

// Line clipping against square
void ClipLineAsSquare(HDC hdc,
    int x1, int y1, int x2, int y2,
    int bx1, int by1, int side,
    COLORREF col,
    bool drawBorder)
{
    ClipLineAsRect(hdc, x1, y1, x2, y2,
        bx1, by1, bx1 + side, by1 + side,
        col, drawBorder);
}

// Polygon clipping against square
void ClipPolygonAsSquare(HDC hdc,
    int xs[], int ys[], int n,
    int bx1, int by1, int side,
    COLORREF col,
    bool drawBorder)
{
    ClipPolygonAsRect(hdc, xs, ys, n,
        bx1, by1, bx1 + side, by1 + side,
        col, drawBorder);
}
