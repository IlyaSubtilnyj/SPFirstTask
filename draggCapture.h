#pragma once
#include <windows.h>
#include <assert.h>
#include <memory.h>

POINT drcpt_get_dragged_file_names(HDROP hDrop, OUT wchar_t*** names, OUT int* pcntFilesDropped);