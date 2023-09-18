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
#include "toss_sprite.h";
#include "resource.h"

#define IDT_TIMER1 1000

const wchar_t* const CLASS_NAME = L"SPFirstLabClass";
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
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
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
        L"SPFirstLab",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 1000, 800,

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

    HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCEL1));

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    SetTimer(hwnd, IDT_TIMER1, 25,(TIMERPROC)NULL);     

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        if (!TranslateAccelerator(hwnd, hAccel, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

#if (1/DEBUGCONSOLE == 1) && (1/DEBUG == 1)
    dcFreeConsole();
#endif

    KillTimer(hwnd, IDT_TIMER1);

    DestroyAcceleratorTable(hAccel);

    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    POINT pt;
    POINT dummy = { .x = -1, .y = -1 };
    switch (uMsg)
    {
    case WM_CREATE:
        mdltosssprite.init(hwnd, "v1.0");
        break;
    case WM_DESTROY:
        mdltosssprite.destroy(hwnd);
        PostQuitMessage(4);
        break;
    case WM_DROPFILES:
        wchar_t** names = NULL;
        int lnamesSize = 0;
        pt = drcpt_get_dragged_file_names(wParam, &names, &lnamesSize);
        for (size_t i = 0; i < lnamesSize; i++)
        {
            int pointer_handler = mdltosssprite.sprite.create(names[i], pt);
            POINT _pointer_coo = { .x = 190, .y = 190 };
            int sprite_handler = mdltosssprite.pointer.create(_pointer_coo);
            mdltosssprite.bind_sprite_to_pointer(pointer_handler, sprite_handler);
        }
        break;
    case WM_MOUSEMOVE:
        int x = 0;
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        RECT rect;
        HDC memBufDC /* device context for primary drawing */, 
            memDoubleBufDC /* device context for drawing bitmaps (sprutes)*/;
        HBITMAP memBM;

        HDC hdc = BeginPaint(hwnd, &ps);

        /*
        * Initialization
        */
        memBufDC = CreateCompatibleDC(hdc);
        memDoubleBufDC = CreateCompatibleDC(hdc);
        GetClientRect(hwnd, &rect);
        memBM = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
        SelectObject(memBufDC, memBM);
        FillRect(memBufDC, &rect, (HBRUSH)(COLOR_WINDOW + 1));
        /* end initialization */
         
        /* (*1)
        * Drawiong sprites by passing memory device context into drawing function
        * Draw before pointers (*2)
        */
        mdltosssprite.sprite.draw(memBufDC, memDoubleBufDC);
        /* Were used double memoty buferization */

        /* (*2)
        * Drawing points if necessary;
        * Draw after sprites (*1)
        */
        mdltosssprite.pointer.draw(memBufDC);
        /* end paint */

        /*
        * Final move: copy to window device context all painted
        */
        BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, memBufDC, 0, 0, SRCCOPY);
        /* to this moment all is already displayed on clien area */

        /*
        * Free allocated stuff for drawing
        */
        DeleteObject(memBM);
        DeleteDC(memBufDC);
        DeleteDC(memDoubleBufDC);
        /* end releasing*/

        EndPaint(hwnd, &ps);

        break;
    }
    case WM_LBUTTONDOWN:
    {
        pt.x = LOWORD(lParam);
        pt.y = HIWORD(lParam);
        mdltosssprite.pointer.clutch(1, pt);
        break;
    }
    case WM_LBUTTONUP:
    {
        POINT pt;
        pt.x = LOWORD(lParam);
        pt.y = HIWORD(lParam);
        mdltosssprite.pointer.release(1, pt);
        break;
    }
    case WM_KEYDOWN:
    {
        mdltosssprite.pointer.get_moving(wParam);
        break;
    }
    case WM_KEYUP:
    {
        mdltosssprite.pointer.stop(wParam);
        break;
    }
    case WM_TIMER:
    {
        switch (wParam)
        {
        case IDT_TIMER1:
            mdltosssprite.pointer.move(0.025f);
            mdltosssprite.sprite.move(1, 0.025f);
            InvalidateRect(hwnd, NULL, FALSE);
            break;
        }
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case ID_RELEASE:
            mdltosssprite.pointer.release(1, dummy);
            break;
        case ID_CLUTCH:
            mdltosssprite.pointer.clutch(1, dummy);
            break;
        }
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
