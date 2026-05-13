#include "CircleAlgorithms.h"
#include <cmath>

// Draw 8 symmetric points of a circle
void Draw8Points(HDC hdc,
                 int xc,
                 int yc,
                 int x,
                 int y,
                 COLORREF color)
{
    // First quadrant reflections
    SetPixel(hdc, xc + x, yc + y, color);
    SetPixel(hdc, xc - x, yc + y, color);
    SetPixel(hdc, xc + x, yc - y, color);
    SetPixel(hdc, xc - x, yc - y, color);

    // Swap x and y for remaining octants
    SetPixel(hdc, xc + y, yc + x, color);
    SetPixel(hdc, xc - y, yc + x, color);
    SetPixel(hdc, xc + y, yc - x, color);
    SetPixel(hdc, xc - y, yc - x, color);
}



// Direct Circle Algorithm
// Uses circle equation: x^2 + y^2 = r^2
void DirectCircle(HDC hdc, int xc, int yc, int r, COLORREF color)
{
    int x = 0;
    int y = r;
    int R2 = r * r;

    // First point (top of circle)
    Draw8Points(hdc, xc, yc, x, y, color);

    // Generate circle points from equation
    while (x < y)
    {
        x++;
        y = (int)round(sqrt((double)(R2 - x * x)));

        Draw8Points(hdc, xc, yc, x, y, color);
    }
}

// Midpoint Circle Algorithm
void MidpointCircle(HDC hdc, int xc, int yc, int r, COLORREF color)
{
    int x = 0;
    int y = r;
    int d = 1 - r;

    // Initial point
    Draw8Points(hdc, xc, yc, x, y, color);

    // Traverse first octant
    while (x < y)
    {
        if (d < 0)
        {
            // Move East
            d += 2 * x + 2;
        }
        else
        {
            // Move South-East
            d += 2 * (x - y) + 5;
            y--;
        }

        x++;

        Draw8Points(hdc, xc, yc, x, y, color);
    }
}

// Modified Midpoint Circle Algorithm
void ModifiedMidpointCircle(HDC hdc, int xc, int yc, int r, COLORREF color)
{
    int x = 0;
    int y = r;

    int d = 1 - r;
    int ch1 = 3;
    int ch2 = 5 - (2 * r);

    Draw8Points(hdc, xc, yc, x, y, color);

    while (x < y)
    {
        if (d < 0)
        {
            // Choose E
            d += ch1;
            ch2 += 2;
        }
        else
        {
            // Choose SE
            d += ch2;
            ch2 += 4;
            y--;
        }

        ch1 += 2;
        x++;

        Draw8Points(hdc, xc, yc, x, y, color);
    }
}

void PolarCircle(HDC hdc, int xc, int yc, int r, COLORREF color)
{
    float theta;
    float x, y;

    for (theta = 0; theta <= 2 * 3.14159; theta += 0.01)
    {
        x = r * cos(theta);
        y = r * sin(theta);
        SetPixel(hdc, xc + (int)x, yc + (int)y, color);
    }
}

void IterativePolarCircle(HDC hdc, int xc, int yc, int r, COLORREF color)
{
    float x = r;
    float y = 0;

    float dtheta = 0.01;

    float cosTheta = cos(dtheta);
    float sinTheta = sin(dtheta);

    float x_new, y_new;

    for (float theta = 0; theta <= 2 * 3.14159; theta += dtheta)
    {
        SetPixel(hdc, xc + (int)x, yc + (int)y, color);

        x_new = x * cosTheta - y * sinTheta;
        y_new = x * sinTheta + y * cosTheta;

        x = x_new;
        y = y_new;
    }
}
