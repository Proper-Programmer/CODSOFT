#include <windows.h>
#include <string>
#include <cstdio>

HWND hPreview, hNum1, hNum2, hResult;
char op = 0;

double compute(double a, double b) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0 ? a / b : 0;
    }
    return 0;
}

void updatePreview() {
    char buf[100];
    GetWindowTextA(hNum1, buf, 100);
    std::string text = buf;
    if (op) {
        text.push_back(op);
        GetWindowTextA(hNum2, buf, 100);
        text += buf;
    }
    SetWindowTextA(hPreview, text.c_str());
}

bool getDoubleFromEdit(HWND hEdit, double &out) {
    char text[100];
    GetWindowTextA(hEdit, text, 100);
    try {
        out = std::stod(text);
        return true;
    } catch (...) {
        return false;
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        hPreview = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY,
                                 20, 5, 380, 20, hwnd, NULL, NULL, NULL);
        CreateWindowA("STATIC", "First Number:", WS_VISIBLE | WS_CHILD,
                      20, 35, 80, 20, hwnd, NULL, NULL, NULL);
        hNum1 = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                              110, 35, 100, 20, hwnd, NULL, NULL, NULL);
        CreateWindowA("BUTTON", "+", WS_VISIBLE | WS_CHILD,
                      220, 35, 30, 20, hwnd, (HMENU)1, NULL, NULL);
        CreateWindowA("BUTTON", "-", WS_VISIBLE | WS_CHILD,
                      260, 35, 30, 20, hwnd, (HMENU)2, NULL, NULL);
        CreateWindowA("BUTTON", "*", WS_VISIBLE | WS_CHILD,
                      300, 35, 30, 20, hwnd, (HMENU)3, NULL, NULL);
        CreateWindowA("BUTTON", "/", WS_VISIBLE | WS_CHILD,
                      340, 35, 30, 20, hwnd, (HMENU)4, NULL, NULL);
        CreateWindowA("STATIC", "Second Number:", WS_VISIBLE | WS_CHILD,
                      20, 65, 80, 20, hwnd, NULL, NULL, NULL);
        hNum2 = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                              110, 65, 100, 20, hwnd, NULL, NULL, NULL);
        CreateWindowA("BUTTON", "=", WS_VISIBLE | WS_CHILD,
                      20, 95, 180, 30, hwnd, (HMENU)5, NULL, NULL);
        CreateWindowA("BUTTON", "Reset", WS_VISIBLE | WS_CHILD,
                      210, 95, 180, 30, hwnd, (HMENU)6, NULL, NULL);
        hResult = CreateWindowA("STATIC", "", WS_VISIBLE | WS_CHILD,
                                20, 135, 380, 20, hwnd, NULL, NULL, NULL);
        break;
    case WM_COMMAND:
        if (HIWORD(wParam) == EN_CHANGE) {
            HWND src = (HWND)lParam;
            if (src == hNum1 || src == hNum2) {
                updatePreview();
            }
        } else {
            switch (LOWORD(wParam)) {
                case 1: op = '+'; updatePreview(); break;
                case 2: op = '-'; updatePreview(); break;
                case 3: op = '*'; updatePreview(); break;
                case 4: op = '/'; updatePreview(); break;
                case 5: {
                    double a, b;
                    if (!getDoubleFromEdit(hNum1, a) || !getDoubleFromEdit(hNum2, b) || op == 0) {
                        SetWindowTextA(hResult, "Invalid calculation");
                    } else {
                        double res = compute(a, b);
                        char buf[100];
                        std::snprintf(buf, sizeof(buf), "%.6f", res);
                        SetWindowTextA(hResult, buf);
                    }
                    break;
                }
                case 6: {
                    op = 0;
                    SetWindowTextA(hNum1, "");
                    SetWindowTextA(hNum2, "");
                    updatePreview();
                    SetWindowTextA(hResult, "");
                    break;
                }
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "CalculatorClass";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    RegisterClassA(&wc);
    HWND hwnd = CreateWindowA(
        wc.lpszClassName, "Calculator",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
        CW_USEDEFAULT, CW_USEDEFAULT, 420, 200,
        NULL, NULL, hInstance, NULL
    );
    ShowWindow(hwnd, nCmdShow);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}