#ifndef UNICODE
#define UNICODE
#endif 
#pragma warning(disable : 4996)

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
/*
    be careful in release mode
    check NDEBUGE disallowed
*/
#include <assert.h> 

#define SININST

const wchar_t* const CLASS_NAME = L"Sample Window Class";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int reRunningCheck() {
    FILE* loggingStream;
    errno_t err;
    char checkInfo[1000];
    int result = EXIT_SUCCESS;

    HANDLE hGlobalMutex = CreateMutex(NULL, TRUE, TEXT("Global\\instantioation-check-mutex"));
    DWORD lastError = GetLastError();

    assert(hGlobalMutex && "Error creating mutex");

    sprintf(checkInfo, "CreateMutex => HANDLE : %i; GetLastError() => DWORD : %lu\n", hGlobalMutex, lastError);

    err = fopen_s(&loggingStream, "logging.c", "w+");
    assert(err == 0 && "The file 'logging.c' was not opened");

    fwrite(checkInfo, strlen(checkInfo), 1, loggingStream);

    if (lastError == ERROR_ALREADY_EXISTS) {
        fwrite("Mutex already in use! Cannot run.\n", sizeof "Mutex already in use! Cannot run.\n", 1, loggingStream);
        result = EXIT_FAILURE;
    }
    else {
        fwrite("This is first instance.\n", sizeof "This is first instance.\n", 1, loggingStream);
    }
    if (loggingStream) {
        if (fclose(loggingStream)) {
            assert(0 && "The file 'logging.c' was not closed");
        }
    }
    if (lastError != ERROR_SUCCESS) {
        //why is this needed?
        //https://stackoverflow.com/questions/54152660/createmutex-seems-to-acquire-mutex-even-when-error-already-exists
        if (hGlobalMutex != NULL)
        {
            ReleaseMutex(hGlobalMutex);
            CloseHandle(hGlobalMutex);
        }
    }
    return result;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

#ifdef SININST
    int reRunningCheckRes = reRunningCheck();
    assert(reRunningCheckRes == EXIT_SUCCESS && "seems like program already executing");
#endif // SINGLE WINAPP INSTANTIATION

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
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

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
        0,                              // Optional window styles.
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

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
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