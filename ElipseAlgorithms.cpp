#include "ElipseAlgorithms.h"
#include <cmath>
#include <algorithm>

void Draw4Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color)
{
    SetPixel(hdc, xc + x, yc + y, color);
    SetPixel(hdc, xc - x, yc + y, color);
    SetPixel(hdc, xc + x, yc - y, color);
    SetPixel(hdc, xc - x, yc - y, color);
}

void DirectEllipse(HDC hdc, int xc, int yc, int A, int B, COLORREF color)
{
    int x = 0;
    int y;

    double A2 = A * A;

    while (x <= A)
    {
        y = (int)(B * sqrt(1 - ((double)(x * x) / A2)));

    
        SetPixel(hdc, xc + x, yc + y, color);
        SetPixel(hdc, xc - x, yc + y, color);
        SetPixel(hdc, xc + x, yc - y, color);
        SetPixel(hdc, xc - x, yc - y, color);

        x++;
    }
}

void PolarEllipse(HDC hdc, int xc, int yc, int A, int B, COLORREF color)
{
  double dtheta = 1.0 / std::max(A, B); 
    for (double theta = 0; theta <= 1.5708; theta += dtheta) // 1.5708 is PI/2
    {
        int x = (int)(A * cos(theta) + 0.5);
        int y = (int)(B * sin(theta) + 0.5);
        Draw4Points(hdc, xc, yc, x, y, color);
    }

}


void MidpointEllipse(HDC hdc, int xc, int yc, int rx, int ry, COLORREF color)
{
    int x = 0;
    int y = ry;

    float rx2 = rx * rx;
    float ry2 = ry * ry;

    float d1 = ry2 - (rx2 * ry) + (0.25 * rx2);
    float dx = 2 * ry2 * x;
    float dy = 2 * rx2 * y;

   
    while (dx < dy)
    {
        Draw4Points(hdc, xc, yc, x, y, color);

        if (d1 < 0)
        {
            x++;
            dx += 2 * ry2;
            d1 += dx + ry2;
        }
        else
        {
            x++;
            y--;
            dx += 2 * ry2;
            dy -= 2 * rx2;
            d1 += dx - dy + ry2;
        }
    }

    float d2 = (ry2 * (x + 0.5) * (x + 0.5)) +
               (rx2 * (y - 1) * (y - 1)) -
               (rx2 * ry2);

    while (y >= 0)
    {
        Draw4Points(hdc, xc, yc, x, y, color);

        if (d2 > 0)
        {
            y--;
            dy -= 2 * rx2;
            d2 += rx2 - dy;
        }
        else
        {
            x++;
            y--;
            dx += 2 * ry2;
            dy -= 2 * rx2;
            d2 += dx - dy + rx2;
        }
    }
}