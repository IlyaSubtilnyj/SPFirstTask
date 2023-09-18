#define POINTERIMPL
#include "pointer.h"
#undef POINTERIMPL

struct _pointer {
	struct _coo destination;
	float speed;
	char left;
	char right;
	char up;
	char down;
};

static struct _pointer object = { .destination = { 0 }, .speed = 24, .left = 0, .right = 0, .up = 0, .down = 0 };

int pointer_create(POINT p_coo) {
	object.destination.x = (float)p_coo.x;
	object.destination.y = (float)p_coo.y;
}

void pointer_get_moving(int p_direction) {
	switch (p_direction)
	{
	case VK_LEFT:
		object.left = 1;
		break;
	case VK_RIGHT:
		object.right = 1;
		break;
	case VK_UP:
		object.up = 1;
		break;
	case VK_DOWN:
		object.down = 1;
		break;
	default:
		break;
	}
}

void pointer_move(float time) {
	if (object.left) object.destination.x -= object.speed;
	if (object.right) object.destination.x += object.speed;
	if (object.up) object.destination.y -= object.speed;
	if (object.down) object.destination.y += object.speed;
}

void pointer_stop(int p_direction) {
	switch (p_direction)
	{
	case VK_LEFT:
		object.left = 0;
		break;
	case VK_RIGHT:
		object.right = 0;
		break;
	case VK_UP:
		object.up = 0;
		break;
	case VK_DOWN:
		object.down = 0;
		break;
	default:
		object.left = 0;
		object.right = 0;
		object.down = 0;
		object.up = 0;
		break;
	}
}

void pointer_clutch() {

}

void pointer_draw(HDC hdc) {
	SetPixel(hdc, object.destination.x, object.destination.y, RGB(0, 0, 255));
	SetPixel(hdc, object.destination.x + 50/*object.size.x*/, object.destination.y, RGB(0, 0, 255));
	SetPixel(hdc, object.destination.x + 50/*object.size.x*/, object.destination.y + 50/*object.size.y*/, RGB(0, 0, 255));
	SetPixel(hdc, object.destination.x, object.destination.y + 50/*object.size.y*/, RGB(0, 0, 255));
}

boolean is_pointer(int pointer_h)
{
	return TRUE;
}

struct _coo get_pointer_destination(int handler)
{
	return object.destination;
}

void set_pointer_destination(struct _coo destination) {
	object.destination = destination;
}

void pointer_release(int h, POINT pt) {

}