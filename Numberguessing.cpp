#include <windows.h>
#include <string>
#include <cstdlib>
#include <ctime>

static int secretNumber;
static HWND hEditGuess, hStaticFeedback, hButtonGuess, hButtonPlay;

void SetText(HWND hWnd, const std::string& s) {
    SetWindowTextA(hWnd, s.c_str());
}

void StartNewGame(HWND hWnd) {
    secretNumber = (rand() % 100) + 1;
    SetText(hStaticFeedback, "");
    SetText(hEditGuess, "");
    EnableWindow(hEditGuess, TRUE);
    EnableWindow(hButtonGuess, TRUE);
    EnableWindow(hButtonPlay, FALSE);
}

void OnGuess(HWND hWndParent) {
    char buf[32]{};
    GetWindowTextA(hEditGuess, buf, sizeof(buf));
    int guess = atoi(buf);

    if (guess < 1 || guess > 100) {
        SetText(hStaticFeedback, "Enter a number between 1 and 100.");
    } else if (guess < secretNumber) {
        SetText(hStaticFeedback, "Too low!");
    } else if (guess > secretNumber) {
        SetText(hStaticFeedback, "Too high!");
    } else {
        SetText(hStaticFeedback, "Correct! You win!");
        EnableWindow(hEditGuess, FALSE);
        EnableWindow(hButtonGuess, FALSE);
        EnableWindow(hButtonPlay, TRUE);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        srand(static_cast<unsigned>(time(nullptr)));
        // Label
        CreateWindowA("STATIC", "I'm thinking of a number between 1 and 100.",
                      WS_VISIBLE | WS_CHILD,
                      10, 10, 350, 20,
                      hWnd, nullptr, nullptr, nullptr);
        // Edit
        hEditGuess = CreateWindowA("EDIT", "",
                                   WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                                   10, 40, 100, 25,
                                   hWnd, nullptr, nullptr, nullptr);
        // Guess Button
        hButtonGuess = CreateWindowA("BUTTON", "Guess",
                      WS_VISIBLE | WS_CHILD,
                      120, 40, 80, 25,
                      hWnd, (HMENU)1001, nullptr, nullptr);
        // Play Again Button
        hButtonPlay = CreateWindowA("BUTTON", "Play Again",
                      WS_VISIBLE | WS_CHILD,
                      210, 40, 100, 25,
                      hWnd, (HMENU)1002, nullptr, nullptr);
        // Feedback
        hStaticFeedback = CreateWindowA("STATIC", "",
                                        WS_VISIBLE | WS_CHILD,
                                        10, 80, 350, 20,
                                        hWnd, nullptr, nullptr, nullptr);
        // Start game
        StartNewGame(hWnd);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 1001:
            OnGuess(hWnd);
            break;
        case 1002:
            StartNewGame(hWnd);
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProcA(hWnd, msg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASSA wc{};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.lpszClassName = "GuessNumberClass";
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    RegisterClassA(&wc);

    HWND hWnd = CreateWindowA(
        wc.lpszClassName,
        "Guess the Number",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 380, 160,
        nullptr, nullptr, hInst, nullptr
    );
    ShowWindow(hWnd, nCmdShow);

    MSG msg;
    while (GetMessageA(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return static_cast<int>(msg.wParam);
}
