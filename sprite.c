#define SPRITEIMPL
#include "sprite.h"
#undef SPRITEIMPL

struct toss_sprite {
	HBITMAP hBitmap;
	BITMAP bitmap;
	struct _coo center;
	struct _coo coordinate;
	struct _coo vector;
};

static struct toss_sprite object = { 0 };

struct _coo sprite_get_center(BITMAP pbitmap);
int sprite_create(wchar_t* pbitmapFileName, POINT pdropCoordinates) 
{
	int result = 0;
	BITMAP lbitmap;
	HBITMAP lhBitmap = (HBITMAP)LoadImage(NULL, pbitmapFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (lhBitmap != NULL) {
		GetObject(lhBitmap, sizeof(lbitmap), &lbitmap);
		object.hBitmap = lhBitmap;
		object.bitmap = lbitmap;
		object.center = sprite_get_center(object.bitmap);
		object.coordinate = cooAdd(utilConvertWinPointToCoo(pdropCoordinates), object.center);
	} else result = GetLastError();
	return result;
}

void sprite_destroy()
{
	DeleteObject(object.hBitmap);
}

void sprite_move(struct _coo potential_coo, float time, char* bumped_code) {
	if (object.hBitmap == NULL) return;
	const float smoothness = 2.0f;
	object.vector = cooSub(potential_coo,object.coordinate);
	struct _coo potential_next_coo = lerp(object.coordinate, potential_coo, smoothness * time);
	struct _coo next_position = bump(potential_next_coo, object.center, bumped_code);
	object.coordinate = next_position;
}

POINT sprite_get_draw_coordinate(struct toss_sprite sprite);
void sprite_draw(HDC towhich, HDC whence) {
	if (object.hBitmap == NULL) return;
	HGDIOBJ hOldBitmap = SelectObject(whence, object.hBitmap);
	POINT draw_coo = sprite_get_draw_coordinate(object);
	BitBlt(towhich, draw_coo.x, draw_coo.y,
		object.bitmap.bmWidth, object.bitmap.bmHeight, whence, 0, 0, SRCCOPY);
	SelectObject(whence, hOldBitmap);
}

boolean is_sprite(int sprite_h)
{
	return TRUE;
}

POINT sprite_get_draw_coordinate(struct toss_sprite sprite) {
	POINT result = { 0 };
	result.x = sprite.coordinate.x - sprite.center.x;
	result.y = sprite.coordinate.y - sprite.center.y;
	return result;
}

struct _coo sprite_get_center(BITMAP pbitmap) {
	struct _coo result = { 0 };
	result.x = (float)pbitmap.bmWidth / 2;
	result.y = (float)pbitmap.bmHeight / 2;
	return result;
}

