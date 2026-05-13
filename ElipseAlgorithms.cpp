#include "ElipseAlgorithms.h"
#include <cmath>
#include <algorithm>

// Draw 4 symmetric points of the ellipse
void Draw4Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color)
{
    SetPixel(hdc, xc + x, yc + y, color);
    SetPixel(hdc, xc - x, yc + y, color);
    SetPixel(hdc, xc + x, yc - y, color);
    SetPixel(hdc, xc - x, yc - y, color);
}

// Direct Ellipse Algorithm
void DirectEllipse(HDC hdc, int xc, int yc, int A, int B, COLORREF color)
{
    int x = 0;
    int y;

    double A2 = A * A;

    while (x <= A)
    {
        // Compute y from ellipse equation
        y = (int)(B * sqrt(1 - ((double)(x * x) / A2)));

        // Draw 4 symmetric points
        Draw4Points(hdc, xc, yc, x, y, color);

        x++;
    }
}

// Polar Ellipse Algorithm
void PolarEllipse(HDC hdc, int xc, int yc, int A, int B, COLORREF color)
{
    double dtheta = 1.0 / max(A, B);

    for (double theta = 0; theta <= 1.5708; theta += dtheta)
    {
        // Parametric equations of ellipse
        int x = (int)(A * cos(theta) + 0.5);
        int y = (int)(B * sin(theta) + 0.5);

        // Draw symmetric points
        Draw4Points(hdc, xc, yc, x, y, color);
    }
}

// Midpoint Ellipse Algorithm
void MidpointEllipse(HDC hdc, int xc, int yc, int rx, int ry, COLORREF color)
{
    int x = 0;
    int y = ry;

    float rx2 = rx * rx;
    float ry2 = ry * ry;

    float d1 = ry2 - (rx2 * ry) + (0.25 * rx2);
    float dx = 2 * ry2 * x;
    float dy = 2 * rx2 * y;

    // Region 1: horizontal movement dominant
    while (dx < dy)
    {
        Draw4Points(hdc, xc, yc, x, y, color);

        if (d1 < 0)
        {
            x++;                // move right
            dx += 2 * ry2;
            d1 += dx + ry2;
        }
        else
        {
            x++; y--;           // move diagonally
            dx += 2 * ry2;
            dy -= 2 * rx2;
            d1 += dx - dy + ry2;
        }
    }

    // Region 2: vertical movement dominant
    float d2 = (ry2 * (x + 0.5) * (x + 0.5)) +
               (rx2 * (y - 1) * (y - 1)) -
               (rx2 * ry2);

    while (y >= 0)
    {
        Draw4Points(hdc, xc, yc, x, y, color);

        if (d2 > 0)
        {
            y--;                // move down
            dy -= 2 * rx2;
            d2 += rx2 - dy;
        }
        else
        {
            x++; y--;           // move diagonally
            dx += 2 * ry2;
            dy -= 2 * rx2;
            d2 += dx - dy + rx2;
        }
    }
}