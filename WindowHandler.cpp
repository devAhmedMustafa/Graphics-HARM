#include "WindowHandler.h"
#include "RenderPipeline.h"
#include "Input.h"
#include "LineRenderAction.h"
#include "CircleRenderAction.h"
#include "EllipseRenderAction.h"
#include "CurveRenderAction.h"
#include "FillingRenderAction.h"
#include "ClippingRenderAction.h"
#include "ShapeStore.h"

#include <sstream>

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

WindowHandler* WindowHandler::instance = nullptr;

HWND WindowHandler::SetupWindow(HINSTANCE instance, int nsh) {
    WNDCLASS wc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hInstance = instance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = L"renderer";
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wc);

    hwnd = CreateWindow(L"renderer", L"Graphics Engine", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, instance, 0);

    ShowWindow(hwnd, nsh);
    UpdateWindow(hwnd);

    return hwnd;
}

LRESULT WINAPI WindowHandler::WndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
{
    auto handler = WindowHandler::getInstance();

    switch (mcode)
    {
    case WM_LBUTTONDOWN: {
        int xPos = GET_X_LPARAM(lp);
        int yPos = GET_Y_LPARAM(lp);

        Input::pushMouseClick({ Point{xPos, yPos}, 0 });
        break;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        auto shapes = ShapeStore::getShapes();

        for (auto& shape : shapes) {
            shape->draw(hdc);
        }

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }


    if (handler->currentAction == nullptr) {
        auto renderPipeline = RenderPipeline::getInstance();
        const auto command = renderPipeline->getRenderCommand();
        if (command.has_value()) {
            switch (command->mode)
            {
            case RenderMode::ChangeBgToWhite:
				SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)GetStockObject(WHITE_BRUSH));
				InvalidateRect(hwnd, nullptr, TRUE);
                break;

            case RenderMode::SetDrawingColor: {
                // Parse color from command.msg and set it as the current drawing color
                auto msg = command->msg;
                auto colors = split(msg, ' ');
                int r = std::stoi(colors[0]), g = std::stoi(colors[1]), b = std::stoi(colors[2]);
                COLORREF color = RGB(r, g, b);
                renderPipeline->setDrawingColor(color);
                break;
            }

            case RenderMode::DrawLine:
				handler->currentAction = new LineRenderAction(command.value());
                break;
            case RenderMode::DrawCircle:
                handler->currentAction = new CircleRenderAction(command.value());
                break;
            case RenderMode::DrawEllipse:
				handler->currentAction = new EllipseRenderAction(command.value());
                break;
            case RenderMode::DrawCurve:
                handler->currentAction = new CurveRenderAction(command.value());
				break;
            case RenderMode::Fill:
                handler->currentAction = new FillingRenderAction(command.value());
                break;
            case RenderMode::Clip:
                handler->currentAction = new ClippingRenderAction(command.value());
                break;
            default:
                break;
            }
        }


    }

    if (handler->currentAction != nullptr) {
        if (handler->currentAction->isDone) {
            delete handler->currentAction;
            handler->currentAction = nullptr;
            Input::stopCapturing();
        }
    }

    return DefWindowProc(hwnd, mcode, wp, lp);
}
