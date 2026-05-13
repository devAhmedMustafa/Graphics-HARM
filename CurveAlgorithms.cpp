
#include "CurveAlgorithms.h"
#include <cmath>
void CardinalSpline(HDC hdc, int x0, int x1, int x2, int x3, int y0, int y1, int y2, int y3, COLORREF c)
{
    float s = 0.01f;
    float T1x, T1y;
    float T2x, T2y;
    T1x = s * (x2 - x0);
    T1y = s * (y2 - y0);
    T2x = s * (x3 - x1);
    T2y = s * (y3 - y1);
    float t, H1, H2, H3, H4;
    float x, y;

    for (t = 0; t <= 1; t += 0.001)
    {
        H1 = 2 * t * t * t - 3 * t * t + 1;
        H2 = -2 * t * t * t + 3 * t * t;
        H3 = t * t * t - 2 * t * t + t;
        H4 = t * t * t - t * t;
        x = H1 * x1 + H2 * x2 + H3 * T1x + H4 * T2x;
        y = H1 * y1 + H2 * y2 + H3 * T1y + H4 * T2y;

        SetPixel(hdc, (int)x, (int)y, c);
    }
}