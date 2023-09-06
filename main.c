#ifndef UNICODE
#define UNICODE
#endif 
#pragma warning(disable : 4996)

#include "config.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
/*
    be careful in release mode
    check NDEBUGE disallowed
*/
#include <assert.h>

#include "runningCheck.h";
#include "debugConsole.h";
#include "draggCapture.h";

const wchar_t* const CLASS_NAME = L"Sample Window Class";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

#if 1/SININST==1
    boolean runningCheck = rcProcessExists();
    assert(runningCheck == 0 && "seems like program already executing");
#endif // SINGLE WINAPP INSTANTIATION

    
#if (1/DEBUGCONSOLE==1) && (1/DEBUG==1)
    dcCreateConsole();
#endif

    // Register the window class.
    WNDCLASSEX wcex = { 0 };
    
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = CLASS_NAME;
    wcex.hIconSm = wcex.hIcon;

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            TEXT("Call to RegisterClassEx failed!"),
            TEXT("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    // Create the window.

    HWND hwnd = CreateWindowEx(
        WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES,   //DragAcceptFiles(hwnd, TRUE)
        CLASS_NAME,                     // Window class
        L"Learn to Program Windows",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (!hwnd)
    {
        MessageBox(NULL,
            TEXT("Call to CreateWindowEx failed!"),
            TEXT("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    // Run the message loop.

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

#if (1/DEBUGCONSOLE == 1) && (1/DEBUG == 1)
    dcFreeConsole();
#endif

    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DROPFILES:
        char** names = NULL;
        drcpt_get_dragged_file_names(wParam, names);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(4);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
