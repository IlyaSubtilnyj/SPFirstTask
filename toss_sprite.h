#ifndef TOSSSPRITE_H
#define TOSSSPRITE_H

#include <windows.h>
#include <assert.h>

struct _toss_sprite_namespace {
    struct {
        int (*create)(wchar_t*, POINT);
        void (*move)(int pointer_h, float time);
        void (*draw)(HDC towhich, HDC whence);
    } sprite;
    struct {
        int (*create)(POINT);
        void (*get_moving)(int);
        void (*move)(int);
        void (*stop)(int);
        void (*clutch)(int, POINT);
        void (*release)(int, POINT);
        void (*draw)(HDC);
    } pointer;
    void (*bind_sprite_to_pointer)(int, int);
    void (*init)(HWND, char*);
    void (*destroy)(HWND);
};

#ifdef TOSSSPRITEIMPL
#include "sprite.h"
#include "pointer.h"
#else
extern struct _toss_sprite_namespace mdltosssprite;
#endif

#endif