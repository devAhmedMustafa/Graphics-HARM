#include "CircleAlgorithms.h"
#include <cmath>

void Draw8Points(HDC hdc,
                 int xc,
                 int yc,
                 int x,
                 int y,
                 COLORREF color)
{
    SetPixel(hdc, xc + x, yc + y, color);
    SetPixel(hdc, xc - x, yc + y, color);
    SetPixel(hdc, xc + x, yc - y, color);
    SetPixel(hdc, xc - x, yc - y, color);

    SetPixel(hdc, xc + y, yc + x, color);
    SetPixel(hdc, xc - y, yc + x, color);
    SetPixel(hdc, xc + y, yc - x, color);
    SetPixel(hdc, xc - y, yc - x, color);
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

void DirectCircle(HDC hdc, int xc, int yc, int r, COLORREF color)
{
    int x = 0;
    int y = r;
    int R2 = r * r;
    Draw8Points(hdc, xc, yc, x, y, color);
    while (x < y)
    {
        
        x++;
        y = (int)round(sqrt((double)(R2 - x * x)));
        Draw8Points(hdc, xc, yc, x, y, color);
    }
}


void ModifiedMidpointCircle(HDC hdc, int xc, int yc, int r, COLORREF color)
{
    int x = 0 ;
    int y = r ;
    int d = 1-r ;
    Draw8Points(hdc, xc, yc, x, y, color);
    while(x<y){
     if(d<0)  
        d+=2*x+2; 
     else  {
        d+=2*(x-y)+5; 
         y--;  } 
    x++; 
    }
  Draw8Points(hdc,xc,yc,x,y,color); 
}
