#include "debugConsole.h";

/*
    part of realization: https://stackoverflow.com/questions/15543571/allocconsole-not-displaying-cout;
    https://www.ibm.com/docs/en/zos/2.2.0?topic=functions-clearerr-reset-error-end-file-eof;
    https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences;
*/

#define CPPREALIZATION

void dcCreateConsole()
{
    if (!AllocConsole()) {
        assert(0 && "AllocConsole fails");
        // Add some error handling here.
        // You can call GetLastError() to get more info about the error.
        return;
    }
    // std::cout, std::clog, std::cerr, std::cin
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    clearerr_s(stdout);
    clearerr_s(stderr);
    clearerr_s(stdin);

    CPPREALIZATION /*
        std::cout.clear();
        std::clog.clear();
        std::cerr.clear();
        std::cin.clear();
    */

    // std::wcout, std::wclog, std::wcerr, std::wcin
    HANDLE hConOut = CreateFile(TEXT("CONOUT$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hConIn = CreateFile(TEXT("CONIN$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
    SetStdHandle(STD_ERROR_HANDLE, hConOut);
    SetStdHandle(STD_INPUT_HANDLE, hConIn);
    clearerr_s(stdout);
    clearerr_s(stdin);
    clearerr_s(stderr);

    CPPREALIZATION /*
        std::wcout.clear();
    std::wclog.clear();
    std::wcerr.clear();
    std::wcin.clear();
    */

    //enabling escape sequences
    HANDLE handleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode;
    GetConsoleMode(hConOut, &consoleMode);
    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    consoleMode |= DISABLE_NEWLINE_AUTO_RETURN;
    SetConsoleMode(hConOut, consoleMode);
}

void dcFreeConsole() {
    if (!FreeConsole())
        MessageBox(NULL, L"Failed to free the console!", NULL, MB_ICONEXCLAMATION);
}