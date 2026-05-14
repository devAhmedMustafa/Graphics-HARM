#include "LineAlgorithms.h"
#include <cmath>
void lineDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF C)
{
    int dx = x2 - x1;
    int dy = y2 - y1;

    if (abs(dy) <= abs(dx))
    {
        if (x1 > x2)
        {
            int temp;
            temp = x1; x1 = x2; x2 = temp;
            temp = y1; y1 = y2; y2 = temp;
        }

        dx = x2 - x1;
        dy = y2 - y1;

        double m = (double)dy / dx;
        double y = y1;

        int x;
        for (x = x1; x <= x2; x++)
        {
            SetPixel(hdc, x, (int)round(y), C);
            y += m;
        }
    }
    else
    {
        if (y1 > y2)
        {
            int temp;

            temp = x1; x1 = x2; x2 = temp;
            temp = y1; y1 = y2; y2 = temp;
        }

        dx = x2 - x1;
        dy = y2 - y1;

        double m = (double)dx / dy;
        double x = x1;

        int y;
        for (y = y1; y <= y2; y++)
        {
            SetPixel(hdc, (int)round(x), y, C);
            x += m;
        }
    }
}
void MidPointLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{
    int x, y, dx, dy, d, dd1, dd2;

    x = x1;
    y = y1;
    dx = x2 - x1;
    dy = y2 - y1;
    SetPixel(hdc, x, y, c);
    d = 2 * dy - dx;
    dd1 = 2 * dx - 2 * dy;
    dd2 = -2 * dy;

    while (x <= x2)
    {
        if (d < 0)
        {
            y++;
            d += dd1;
        }
        else
        {
            d += dd2;
        }

        x++;

        SetPixel(hdc, x, y, c);
    }
}
void ParametricLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{
    float t;
    float x, y;

    for (t = 0; t <= 1; t += 0.001f)
    {
        x = x1 + t * (x2 - x1);
        y = y1 + t * (y2 - y1);

        SetPixel(hdc, (int)x, (int)y, c);
    }
}