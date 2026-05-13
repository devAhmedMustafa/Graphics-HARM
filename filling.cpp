#include "filling.h"

// ========================== CIRCLE FUNCTIONS ==========================

void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color)
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

void DrawCircle(HDC hdc, int xc, int yc, int r, COLORREF color)
{
    int x = 0;
    int y = r;
    int d = 1 - r;

    Draw8Points(hdc, xc, yc, x, y, color);

    while (x < y)
    {
        x++;
        if (d < 0)
            d += 2 * x + 1;
        else
        {
            y--;
            d += 2 * (x - y) + 1;
        }
        Draw8Points(hdc, xc, yc, x, y, color);
    }
}

static void DrawQuarter(const HDC hdc, const int xc, const int yc, const int x, const int y, const int xx,
    const int yy, const COLORREF color)
{
    if (xx > xc && yy > yc)
    {
        SetPixel(hdc, xc + x, yc + y, color);
        SetPixel(hdc, xc + y, yc + x, color);
    }
    if (xx > xc && yy < yc)
    {
        SetPixel(hdc, xc + x, yc - y, color);
        SetPixel(hdc, xc + y, yc - x, color);
    }
    if (xx < xc && yy > yc)
    {
        SetPixel(hdc, xc - x, yc + y, color);
        SetPixel(hdc, xc - y, yc + x, color);
    }
    if (xx < xc && yy < yc)
    {
        SetPixel(hdc, xc - x, yc - y, color);
        SetPixel(hdc, xc - y, yc - x, color);
    }
}

void DrawCircleQuarter(HDC hdc, int xc, int yc, int r, int clickX, int clickY, COLORREF color)
{
    int x = 0;
    int y = r;
    int d = 1 - r;

    DrawQuarter(hdc, xc, yc, x, y, clickX, clickY, color);

    while (x < y)
    {
        x++;
        if (d < 0)
            d += 2 * x + 1;
        else
        {
            y--;
            d += 2 * (x - y) + 1;
        }

        DrawQuarter(hdc, xc, yc, x, y, clickX, clickY, color);
    }
}

int GetQuadrant(int clickX, int clickY, int centerX, int centerY)
{
    int dx = clickX - centerX;
    int dy = clickY - centerY;

    if (dx >= 0 && dy <= 0) return 0; // Top-Right
    if (dx < 0 && dy <= 0) return 1;  // Top-Left
    if (dx < 0 && dy > 0) return 2;   // Bottom-Left
    return 3; // Bottom-Right
}

void FillQuadrant(HDC hdc, int xc, int yc, int radius, int quadrant)
{
    switch (quadrant) {
    case 0: // Top-Right
        for (int y = 0; y <= radius; y += 2)
        {
            int x = (int)sqrt(radius * radius - y * y);
            for (int px = 0; px <= x; px++) {
                SetPixel(hdc, xc + px, yc - y, RGB(255, 0, 0));
            }
        }
        break;

    case 1: // Top-Left
        for (int y = 0; y <= radius; y += 2)
        {
            int x = (int)sqrt(radius * radius - y * y);
            for (int px = 0; px <= x; px++) {
                SetPixel(hdc, xc - px, yc - y, RGB(255, 0, 0));
            }
        }
        break;

    case 2: // Bottom-Left
        for (int y = 0; y <= radius; y += 2)
        {
            int x = (int)sqrt(radius * radius - y * y);
            for (int px = 0; px <= x; px++) {
                SetPixel(hdc, xc - px, yc + y, RGB(255, 0, 0));
            }
        }
        break;

    case 3: // Bottom-Right
        for (int y = 0; y <= radius; y += 2)
        {
            int x = (int)sqrt(radius * radius - y * y);
            for (int px = 0; px <= x; px++) {
                SetPixel(hdc, xc + px, yc + y, RGB(255, 0, 0));
            }
        }
        break;
    }
}

// ========================== LINE FUNCTIONS ==========================

void DrawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color)
{
    int dx = x2 - x1;
    int dy = y2 - y1;

    int steps = max(abs(dx), abs(dy));

    double xInc = dx / (double)steps;
    double yInc = dy / (double)steps;

    double x = x1;
    double y = y1;

    for (int i = 0; i <= steps; i++)
    {
        SetPixel(hdc, round(x), round(y), color);
        x += xInc;
        y += yInc;
    }
}

void DrawLineDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color)
{
    int dx = x2 - x1;
    int dy = y2 - y1;

    if (abs(dx) >= abs(dy)) {
        double m = (double)dy / dx;

        if (x1 > x2) {
            swap(x1, x2);
            swap(y1, y2);
        }

        SetPixel(hdc, x1, y1, color);
        double x = x1;
        double y = y1;

        while (x < x2) {
            x++;
            y += m;
            SetPixel(hdc, (int)x, (int)round(y), color);
        }
    }
    else {
        double m = (double)dx / dy;
        if (y1 > y2) {
            swap(x1, x2);
            swap(y1, y2);
        }

        double x = x1;
        double y = y1;

        SetPixel(hdc, x1, y1, color);

        while (y < y2) {
            y++;
            x += m;
            SetPixel(hdc, (int)round(x), (int)y, color);
        }
    }
}

// ========================== RECTANGLE FUNCTIONS ==========================

void DrawRectangle(HDC hdc, int left, int top, int right, int bottom)
{
    DrawLine(hdc, left, top, right, top, RGB(0, 0, 0));
    DrawLine(hdc, right, top, right, bottom, RGB(0, 0, 0));
    DrawLine(hdc, right, bottom, left, bottom, RGB(0, 0, 0));
    DrawLine(hdc, left, bottom, left, top, RGB(0, 0, 0));
}

void DrawSquare(HDC hdc, int left, int top, int right, int bottom)
{
    DrawLine(hdc, left, top, right, top, RGB(0, 0, 0));
    DrawLine(hdc, right, top, right, bottom, RGB(0, 0, 0));
    DrawLine(hdc, right, bottom, left, bottom, RGB(0, 0, 0));
    DrawLine(hdc, left, bottom, left, top, RGB(0, 0, 0));
}

// ========================== BEZIER FUNCTIONS ==========================

void B(HDC hdc, PixelUnsigned p1, PixelUnsigned p2, PixelUnsigned p3, PixelUnsigned p4)
{
    if (p1.distance(p4) < 5)
    {
        SetPixel(hdc, p1.x, p1.y, RGB(255, 0, 0));
        return;
    }

    PixelUnsigned q1 = (p1 + p2) / 2;
    PixelUnsigned q2 = (p2 + p3) / 2;
    PixelUnsigned q3 = (p3 + p4) / 2;

    PixelUnsigned r1 = (q1 + q2) / 2;
    PixelUnsigned r2 = (q2 + q3) / 2;

    PixelUnsigned m = (r1 + r2) / 2;

    B(hdc, p1, q1, r1, m);
    B(hdc, m, r2, q3, p4);
}

void FillBezierHorizontal(HDC hdc, int left, int top, int right, int bottom)
{
    for (int y = top; y <= bottom; y++)
    {
        PixelUnsigned p1 = { (unsigned int)left, (unsigned int)y };
        PixelUnsigned p4 = { (unsigned int)right, (unsigned int)y };

        PixelUnsigned p2 = { (unsigned int)(left + 50), (unsigned int)y };
        PixelUnsigned p3 = { (unsigned int)(right - 50), (unsigned int)y };

        B(hdc, p1, p2, p3, p4);
    }
}

void FillRectangleWithBezierCurves(HDC hdc, Point pts[4])
{
    int numSteps = 25;

    for (int i = 0; i <= numSteps; i++)
    {
        float t = (float)i / numSteps;

        PixelUnsigned leftPoint;
        leftPoint.x = (unsigned int)(pts[0].x + (pts[3].x - pts[0].x) * t);
        leftPoint.y = (unsigned int)(pts[0].y + (pts[3].y - pts[0].y) * t);

        PixelUnsigned rightPoint;
        rightPoint.x = (unsigned int)(pts[1].x + (pts[2].x - pts[1].x) * t);
        rightPoint.y = (unsigned int)(pts[1].y + (pts[2].y - pts[1].y) * t);

        PixelUnsigned cp1, cp2;
        cp1.x = (leftPoint.x + rightPoint.x) / 2;
        cp1.y = leftPoint.y;

        cp2.x = (leftPoint.x + rightPoint.x) / 2;
        cp2.y = rightPoint.y;

        B(hdc, leftPoint, cp1, cp2, rightPoint);
    }
}

// ========================== POLYGON FUNCTIONS (CONVEX) ==========================

void init(Table& table)
{
    for (int i = 0; i < 800; i++)
    {
        table[i].xleft = INT_MAX;
        table[i].xright = INT_MIN;
    }
}

void Edge_to_Table(Point p1, Point p2, Table& table)
{
    if (p1.y == p2.y) return;

    if (p1.y > p2.y) swap(p1, p2);

    int y = p1.y;
    double x = p1.x;
    double mi = (double)(p2.x - p1.x) / (p2.y - p1.y);

    while (y < p2.y)
    {
        if (x < table[y].xleft) table[y].xleft = x;
        if (x > table[y].xright) table[y].xright = x;

        x += mi;
        y++;
    }
}

void Polygon_to_Table(Point P[], int n, Table& t)
{
    Point v1 = P[n - 1];

    for (int i = 0; i < n; i++)
    {
        Point v2 = P[i];
        Edge_to_Table(v1, v2, t);
        v1 = v2;
    }
}

void Table_to_Screen(HDC hdc, Table& t, COLORREF color)
{
    for (int i = 0; i < 800; i++)
    {
        if (t[i].xleft < t[i].xright)
        {
            DrawLineDDA(hdc, (int)t[i].xleft, i, (int)t[i].xright, i, color);
        }
    }
}

void Convex_Fill(HDC hdc, Point P[], int n, COLORREF color)
{
    Table t;
    init(t);
    Polygon_to_Table(P, n, t);
    Table_to_Screen(hdc, t, color);
}

void DrawPolygon(HDC hdc, Point pts[], int n)
{
    for (int i = 0; i < n; i++)
    {
        Point p1 = pts[i];
        Point p2 = pts[(i + 1) % n];

        DrawLineDDA(hdc, p1.x, p1.y, p2.x, p2.y, RGB(0, 0, 0));
    }
}

bool IsPointInPolygon(int x, int y, Point pts[], int n)
{
    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++)
    {
        int xi = pts[i].x, yi = pts[i].y;
        int xj = pts[j].x, yj = pts[j].y;

        bool intersects = ((yi > y) != (yj > y)) &&
            (x < (xj - xi) * (y - yi) / (double)(yj - yi) + xi);

        if (intersects)
            inside = !inside;
    }
    return inside;
}

// ========================== POLYGON FUNCTIONS (NON-CONVEX) ==========================

void init_non_Convex_Table(EdgeTable& table)
{
    for (int i = 0; i < 800; i++)
    {
        table[i] = nullptr;
    }
}

void Edge_to_list(Point p1, Point p2, EdgeTable& table)
{
    if (p1.y == p2.y) return;

    if (p1.y > p2.y) swap(p1, p2);

    EdgeRecord e;
    e.x = p1.x;
    e.y2 = p2.y;
    e.mi = (double)(p2.x - p1.x) / (p2.y - p1.y);

    Node* newNode = new Node{ e, nullptr };

    int y = p1.y;

    if (table[y] == nullptr)
    {
        table[y] = newNode;
    }
    else
    {
        Node* cur = table[y];
        while (cur->next)
            cur = cur->next;

        cur->next = newNode;
    }
}

void Build_Table(Point P[], int n, EdgeTable& t)
{
    Point v1 = P[n - 1];

    for (int i = 0; i < n; i++)
    {
        Point v2 = P[i];
        Edge_to_list(v1, v2, t);
        v1 = v2;
    }
}

void SortList(Node*& head)
{
    if (!head) return;

    for (Node* i = head; i; i = i->next)
    {
        for (Node* j = i->next; j; j = j->next)
        {
            if (i->data.x > j->data.x)
                swap(i->data, j->data);
        }
    }
}

void RemoveFinished(Node*& head, int y)
{
    while (head && head->data.y2 == y)
    {
        Node* temp = head;
        head = head->next;
        delete temp;
    }

    Node* cur = head;
    while (cur && cur->next)
    {
        if (cur->next->data.y2 == y)
        {
            Node* temp = cur->next;
            cur->next = temp->next;
            delete temp;
        }
        else
            cur = cur->next;
    }
}

void UpdateX(Node* head)
{
    while (head)
    {
        head->data.x += head->data.mi;
        head = head->next;
    }
}

void Fill_NonConvex(HDC hdc, Point P[], int n, COLORREF color)
{
    EdgeTable t;
    init_non_Convex_Table(t);
    Build_Table(P, n, t);

    Node* active = nullptr;

    for (int y = 0; y < 800; y++)
    {
        Node* temp = t[y];
        while (temp)
        {
            Node* next = temp->next;
            temp->next = active;
            active = temp;
            temp = next;
        }

        if (!active) continue;

        SortList(active);

        Node* cur = active;
        while (cur && cur->next)
        {
            int x1 = (int)cur->data.x;
            int x2 = (int)cur->next->data.x;

            DrawLineDDA(hdc, x1, y, x2, y, color);

            cur = cur->next->next;
        }

        RemoveFinished(active, y);

        UpdateX(active);
    }
}

// ========================== FLOOD FILL FUNCTIONS ==========================

void non_recursive_flood_fill(HDC hdc, int x, int y, COLORREF bc, COLORREF fc)
{
    std::stack<Point> s;
    s.push(Point(x, y));
    while (!s.empty()) {
        Point p = s.top();
        s.pop();

        COLORREF c = GetPixel(hdc, p.x, p.y);
        if (c == bc || c == fc) continue;
        SetPixel(hdc, p.x, p.y, fc);
        s.push(Point(p.x, p.y + 1));
        s.push(Point(p.x, p.y - 1));
        s.push(Point(p.x - 1, p.y));
        s.push(Point(p.x + 1, p.y));
    }
}

void MyFloodFill(HDC hdc, int x, int y, COLORREF bc, COLORREF fc)
{
    COLORREF C = GetPixel(hdc, x, y);
    if (C == bc || C == fc) return;
    SetPixel(hdc, x, y, fc);
    MyFloodFill(hdc, x + 1, y, bc, fc);
    MyFloodFill(hdc, x - 1, y, bc, fc);
    MyFloodFill(hdc, x, y + 1, bc, fc);
    MyFloodFill(hdc, x, y - 1, bc, fc);
}

// ========================== HERMITE CURVE FUNCTIONS ==========================

Vector4 operator*(Matrix4 M, Vector4& b)
{
    Vector4 res;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            res[i] += M[i][j] * b[j];
    return res;
}

double DotProduct(Vector4& a, Vector4& b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

Vector4 GetHermiteCoeff(double x0, double s0, double x1, double s1)
{
    static double H[16] = { 2,1,-2,1,-3,-2,3,-1,0,1,0,0,1,0,0,0 };
    static Matrix4 basis(H);
    Vector4 v(x0, s0, x1, s1);
    return basis * v;
}

void DrawHermiteCurve(HDC hdc, Vector2 P0, Vector2 T0, Vector2 P1, Vector2 T1, int numpoints, COLORREF color)
{
    Vector4 xcoeff = GetHermiteCoeff(P0.x, T0.x, P1.x, T1.x);
    Vector4 ycoeff = GetHermiteCoeff(P0.y, T0.y, P1.y, T1.y);

    double dt = 1.0 / (numpoints - 1);

    for (double t = 0; t <= 1; t += dt)
    {
        Vector4 vt;
        vt[3] = 1;
        for (int i = 2; i >= 0; i--)
            vt[i] = vt[i + 1] * t;

        int x = round(DotProduct(xcoeff, vt));
        int y = round(DotProduct(ycoeff, vt));

        SetPixel(hdc, x, y, color);
    }
}

void FillHermite(HDC hdc, int left, int top, int right, int bottom)
{
    for (int x = left; x <= right; x += 5)
    {
        Vector2 P0(x, top);
        Vector2 P1(x, bottom);

        Vector2 T0(30, 0);
        Vector2 T1(-30, 0);

        DrawHermiteCurve(hdc, P0, T0, P1, T1, 50, RGB(0, 0, 255));
    }
}

// ========================== UTILITY FUNCTIONS ==========================

void swap(int& p1, int& p2)
{
    int temp = p1;
    p1 = p2;
    p2 = temp;
}

void swap(Point& p1, Point& p2)     
{
    Point temp = p1;
    p1 = p2;
    p2 = temp;
}

void swap(EdgeRecord& p1, EdgeRecord& p2)
{
    EdgeRecord temp = p1;
    p1 = p2;
    p2 = temp;
}