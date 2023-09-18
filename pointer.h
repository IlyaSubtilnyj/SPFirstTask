#ifndef POINTER_H
#define POINTER_H

#include <windows.h>

#include "utils.h"

int pointer_create(POINT p_coo);
void pointer_get_moving(int);
void pointer_move(float);
void pointer_stop(int);
void pointer_clutch(int, POINT);
void pointer_release(int, POINT);
void pointer_draw(HDC);
boolean is_pointer(int);
struct _coo get_pointer_destination(int handler);
void set_pointer_destination(struct _coo destination);


#endif