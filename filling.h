#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <cmath>
#include <stack>
#include <algorithm>

// ========================== STRUCTS ==========================

namespace Filling {
    struct Pixel {
        int x, y;

        int distance(const Pixel& e) const {
            int xDiff = x - e.x;
            int yDiff = y - e.y;
            return (int)sqrt(xDiff * xDiff + yDiff * yDiff);
        }
    };

    struct PixelUnsigned {
        unsigned int x, y;

        unsigned int distance(const PixelUnsigned& e) const {
            auto xDiff = x - e.x;
            auto yDiff = y - e.y;
            return (unsigned int)sqrt(xDiff * xDiff + yDiff * yDiff);
        }

        PixelUnsigned operator +(const PixelUnsigned& e) const {
            return PixelUnsigned{ x + e.x, y + e.y };
        }

        PixelUnsigned operator /(const unsigned int& e) const {
            return PixelUnsigned{ x / e, y / e };
        }
    };

    struct Point {
        int x, y;
        Point(int x = 0, int y = 0) : x(x), y(y) {}
    };

    struct Rec {
        double xleft, xright;
        Rec(double xleft = 0, double xright = 0) : xleft(xleft), xright(xright) {}
    };

    struct EdgeRecord {
        double x;
        int y2;
        double mi;
    };

    struct Node {
        EdgeRecord data;
        Node* next;
    };

    struct Vector2 {
        double x, y;
        Vector2(double a = 0, double b = 0) : x(a), y(b) {}
    };

    class Vector4 {
        double v[4];
    public:
        Vector4(double a = 0, double b = 0, double c = 0, double d = 0) {
            v[0] = a; v[1] = b; v[2] = c; v[3] = d;
        }
        double& operator[](int i) { return v[i]; }
    };

    class Matrix4 {
        Vector4 M[4];
    public:
        Matrix4(double A[]) {
            memcpy(M, A, 16 * sizeof(double));
        }
        Vector4& operator[](int i) { return M[i]; }
    };

    // ========================== TYPEDEFS ==========================

    typedef Rec Table[800];
    typedef Node* EdgeTable[800];

    // ========================== FUNCTION DECLARATIONS ==========================

    // --------- Circle Functions ---------
    void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color);
    void DrawCircle(HDC hdc, int xc, int yc, int r, COLORREF color);
    void DrawCircleQuarter(HDC hdc, int xc, int yc, int r, int clickX, int clickY, COLORREF color);
    int GetQuadrant(int clickX, int clickY, int centerX, int centerY);
    void FillQuadrant(HDC hdc, int xc, int yc, int radius, int quadrant);

    // --------- Line Functions ---------
    void DrawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color);
    void DrawLineDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color);

    // --------- Rectangle Functions ---------
    void DrawRectangle(HDC hdc, int left, int top, int right, int bottom);

    // --------- Bezier Functions ---------
    void B(HDC hdc, PixelUnsigned p1, PixelUnsigned p2, PixelUnsigned p3, PixelUnsigned p4);
    void FillBezierHorizontal(HDC hdc, int left, int top, int right, int bottom);
    void FillRectangleWithBezierCurves(HDC hdc, Point pts[4]);

    // --------- Polygon Functions (Convex) ---------
    void init(Table& table);
    void Edge_to_Table(Point p1, Point p2, Table& table);
    void Polygon_to_Table(Point P[], int n, Table& t);
    void Table_to_Screen(HDC hdc, Table& t, COLORREF color);
    void Convex_Fill(HDC hdc, Point P[], int n, COLORREF color);
    void DrawPolygon(HDC hdc, Point pts[], int n);
    bool IsPointInPolygon(int x, int y, Point pts[], int n);

    // --------- Polygon Functions (Non-Convex) ---------
    void init_non_Convex_Table(EdgeTable& table);
    void Edge_to_list(Point p1, Point p2, EdgeTable& table);
    void Build_Table(Point P[], int n, EdgeTable& t);
    void SortList(Node*& head);
    void RemoveFinished(Node*& head, int y);
    void UpdateX(Node* head);
    void Fill_NonConvex(HDC hdc, Point P[], int n, COLORREF color);

    // --------- Flood Fill Functions ---------
    void non_recursive_flood_fill(HDC hdc, int x, int y, COLORREF bc, COLORREF fc);
    void MyFloodFill(HDC hdc, int x, int y, COLORREF bc, COLORREF fc);

    // --------- Hermite Curve Functions ---------
    Vector4 operator*(Matrix4 M, Vector4& b);
    double DotProduct(Vector4& a, Vector4& b);
    Vector4 GetHermiteCoeff(double x0, double s0, double x1, double s1);
    void DrawHermiteCurve(HDC hdc, Vector2 P0, Vector2 T0, Vector2 P1, Vector2 T1, int numpoints, COLORREF color);
    void FillHermite(HDC hdc, int left, int top, int right, int bottom);

    // --------- Utility Functions ---------
    void swap(int& p1, int& p2);
    void swap(Point& p1, Point& p2);
    void swap(EdgeRecord& p1, EdgeRecord& p2);

    // --------- Square Functions ---------
    void DrawSquare(HDC hdc, int left, int top, int right, int bottom);
}