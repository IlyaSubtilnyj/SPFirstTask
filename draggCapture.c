#include "draggCapture.h"
#include <stdio.h>
#define RESULT

POINT drcpt_get_dragged_file_names(HDROP hDrop, OUT wchar_t*** names, OUT int* pcntFilesDropped) {
    printf("%s", "\x1b[31m");
    printf("#debug[%d]:", __LINE__);
    printf("%s", "\x1b[37m");
    printf("in drcpt_get_dragged_file_names\n");
    int cntFilesDropped;
    RESULT POINT releaseDragPoint;

    DragQueryPoint(hDrop, &releaseDragPoint);

    cntFilesDropped = DragQueryFile(hDrop, 0xffffffff, NULL, 0);

    if ((*names = (wchar_t**)malloc(cntFilesDropped * sizeof(wchar_t*))) == NULL) {
        assert(0 && "GetDroppedFileNames malloc fail");
    }
    for (size_t i = 0; i < cntFilesDropped; i++)
    {
        UINT bufferSize = DragQueryFile(hDrop, i, NULL, 0);
        if (((*names)[i] = (wchar_t*)malloc((bufferSize + 1) * sizeof(wchar_t))) == NULL) {
            assert(0 && "GetDroppedFileNames malloc fail");
        } else 
            DragQueryFile(hDrop, i, (*names)[i], bufferSize + 1);
    }

    *pcntFilesDropped = cntFilesDropped;

    DragFinish(hDrop);

    return releaseDragPoint;
}