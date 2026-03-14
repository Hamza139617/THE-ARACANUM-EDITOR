#include <windows.h>
#include "Header.h"


// globals 
wchar_t inputBuffer[10] = L"";
int inputIndex = 0;
int currentStep = 0;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {

    case WM_CHAR: {

        if (appSettings.getState() == false) {
            
            if (wParam == VK_RETURN) {
                int value = _wtoi(inputBuffer);

                if (currentStep == 0) appSettings.setColumns(value);
                else if (currentStep == 1) appSettings.setLines(value);
                else if (currentStep == 2) {
                    appSettings.setChar(value);
                    appSettings.setState(true);
                    appSettings.validSettings();
                }

                currentStep++;
                inputIndex = 0;
                inputBuffer[0] = L'\0';
            }
            else if (wParam == VK_BACK) {
                if (inputIndex > 0) {
                    inputIndex--;
                    inputBuffer[inputIndex] = L'\0';
                }
            }
            else if (wParam >= L'0' && wParam <= L'9') {
                if (inputIndex < 9) {
                    inputBuffer[inputIndex++] = (wchar_t)wParam;
                    inputBuffer[inputIndex] = L'\0';
                }
            }

            InvalidateRect(hwnd, NULL, TRUE);
        }
        return 0;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);



        RECT rect;
        GetClientRect(hwnd, &rect);


        HBRUSH bgBrush = CreateSolidBrush(RGB(255, 255, 255));
        FillRect(hdc, &rect, bgBrush);
        DeleteObject(bgBrush);

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0, 0, 0));


        if(appSettings.getState() == false ) {

            HFONT font = CreateFont(40, 0, 0, 0, 600, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Times New Roman");
            SelectObject(hdc, font);


            const wchar_t* text = L"THE ARACANUM EDITOR.";
            TextOutW(hdc, 450, 20, text, lstrlenW(text));

            DeleteObject(font);

            const wchar_t* question = L"";

            if (currentStep == 0) question = L"Input your number of Columns : ";
            else if (currentStep == 1) question = L"Input your number of Lines : ";
            else if (currentStep == 2) question = L"Input your number of Characters: ";
            
            TextOutW(hdc, 430, 150, question, lstrlenW(question));
            SetTextColor(hdc, RGB(0, 0, 0));

            TextOutW(hdc, 970, 150, inputBuffer, lstrlenW(inputBuffer));

            MoveToEx(hdc, 970, 190, NULL);
            LineTo(hdc, 1140, 190);

            const wchar_t* prompt = L"Settings Screen ";
            TextOutW(hdc, 534, 80, prompt, lstrlenW(prompt));



        }
        else {

            

            HFONT font = CreateFont(40, 0, 0, 0, 600, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Times New Roman");
            SelectObject(hdc, font);


            const wchar_t* text = L"THE ARACANUM EDITOR.";
            DrawText(hdc, text, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


            DeleteObject(font);
        }

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"FastianWindow";


    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);


    RegisterClassW(&wc);


    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Blackwood & Hargrove",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1440, 720,
        NULL, NULL, hInstance, NULL
    );


    if (!hwnd) return 0;


    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);


    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }


    return 0;
}