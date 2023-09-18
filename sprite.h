#ifndef SPRITE_H
#define SPRITE_H

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

#include "utils.h"

int sprite_create(wchar_t* pbitmapFileName, POINT pdropCoordinates);
void sprite_destroy();
void sprite_move(struct _coo, float time, char*);
void sprite_draw(HDC towhich, HDC whence);
boolean is_sprite(int);

#endif