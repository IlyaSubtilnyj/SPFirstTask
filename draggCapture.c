#include "draggCapture.h"

#define RESULT

POINT drcpt_get_dragged_file_names(HDROP hDrop, OUT char** names) {
    char fName[MAX_PATH];
    int cntFilesDropped;
    RESULT POINT releaseDragPoint;

    DragQueryPoint(hDrop, &releaseDragPoint);

    cntFilesDropped = DragQueryFile(hDrop, 0xffffffff, NULL, 0);

    if ((names = (char**)malloc(cntFilesDropped * sizeof(char*))) == NULL) {
        assert(0 && "GetDroppedFileNames malloc fail");
    }
    for (size_t i = 0; i < cntFilesDropped; i++)
    {
        int symCnt = DragQueryFileA(hDrop, i, fName, sizeof fName);
        if ((names[i] = (char*)malloc(symCnt * sizeof(char))) == NULL) {
            //Add some error handling here
            assert(0 && "GetDroppedFileNames malloc fail");
        }
        errno_t err = memcpy_s(names[i], symCnt, fName, (rsize_t)symCnt);
        assert(err == 0 && "Error executing memcpy_s");
    }

    DragFinish(hDrop);

    return releaseDragPoint;
}