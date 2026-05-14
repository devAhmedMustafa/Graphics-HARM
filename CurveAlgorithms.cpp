#include "CurveAlgorithms.h"
#include <cmath>

void CardinalSpline(HDC hdc,
    int x0, int y0,
    int x1, int y1,
    int x2, int y2,
    int x3, int y3,
    COLORREF c)
{
    float s = 0.5f;

    float T1x = s * (x2 - x0);
    float T1y = s * (y2 - y0);
    float T2x = s * (x3 - x1);
    float T2y = s * (y3 - y1);

    // Estimate curve length from control point chord distances
    float d01 = sqrtf((float)((x1-x0)*(x1-x0) + (y1-y0)*(y1-y0)));
    float d12 = sqrtf((float)((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)));
    float d23 = sqrtf((float)((x3-x2)*(x3-x2) + (y3-y2)*(y3-y2)));

    // Use chord length sum as upper bound, x2 safety factor to avoid gaps
    int steps = (int)((d01 + d12 + d23) * 2);
    if (steps < 1) steps = 1;

    float dt = 1.0f / steps;

    for (float t = 0; t <= 1.0f; t += dt)
    {
        float t2 = t * t;
        float t3 = t2 * t;

        float H1 = 2*t3 - 3*t2 + 1;
        float H2 = -2*t3 + 3*t2;
        float H3 = t3 - 2*t2 + t;
        float H4 = t3 - t2;

        float x = H1*x1 + H2*x2 + H3*T1x + H4*T2x;
        float y = H1*y1 + H2*y2 + H3*T1y + H4*T2y;

        SetPixel(hdc, (int)round(x), (int)round(y), c);
    }
}