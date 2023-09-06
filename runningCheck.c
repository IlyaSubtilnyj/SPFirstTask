#include "runningCheck.h";

/*
    part of realization: https://stackoverflow.com/questions/54152660/createmutex-seems-to-acquire-mutex-even-when-error-already-exists;
*/

boolean rcProcessExists() {
    FILE* loggingStream;
    errno_t err;
    char checkInfo[1000];
    boolean result = FALSE;

    HANDLE hGlobalMutex = CreateMutex(NULL, TRUE, TEXT("Global\\instantioation-check-mutex"));
    DWORD lastError = GetLastError();

    assert(hGlobalMutex && "Error creating mutex");

    sprintf(checkInfo, "CreateMutex => HANDLE : %i; GetLastError() => DWORD : %lu\n", hGlobalMutex, lastError);

    err = fopen_s(&loggingStream, "logging.c", "w+");
    assert(err == 0 && "The file 'logging.c' was not opened");

    fwrite(checkInfo, strlen(checkInfo), 1, loggingStream);

    if (lastError == ERROR_ALREADY_EXISTS) {
        fwrite("Mutex already in use! Cannot run.\n", sizeof "Mutex already in use! Cannot run.\n", 1, loggingStream);
        result = TRUE;
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
        if (hGlobalMutex != NULL)
        {
            ReleaseMutex(hGlobalMutex);
            CloseHandle(hGlobalMutex);
        }
    }
    return result;
}