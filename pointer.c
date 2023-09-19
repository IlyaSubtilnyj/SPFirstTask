#define POINTERIMPL
#include "pointer.h"
#undef POINTERIMPL

#define id 1
#define POINTERRADIUS 15
#define POINTERDEFAULTSPEED 24

static struct _pointer {
	struct _coo destination;
	float speed;
	boolean move_to_left;
	boolean move_to_right;
	boolean move_to_up;
	boolean move_to_down;
	struct _coo size;
};

static struct _pointer object = {
	.destination = { 0 },
	.speed = POINTERDEFAULTSPEED,
	.move_to_left = 0,
	.move_to_right = 0,
	.move_to_up = 0,
	.move_to_down = 0,
	.size = { 0 }
};

int pointer_create(POINT pcoo) {
	object.destination.x = (float)pcoo.x;
	object.destination.y = (float)pcoo.y;
	return id;
}

void pointer_get_moving(int p_direction) {
	switch (p_direction)
	{
	case VK_LEFT:
		object.move_to_left = TRUE;
		break;
	case VK_RIGHT:
		object.move_to_right = TRUE;
		break;
	case VK_UP:
		object.move_to_up = TRUE;
		break;
	case VK_DOWN:
		object.move_to_down = TRUE;
		break;
	default:
		break;
	}
}

void pointer_move(float time) {
	if (object.move_to_left) 
		object.destination.x -= object.speed;
	if (object.move_to_right) 
		object.destination.x += object.speed;
	if (object.move_to_up) 
		object.destination.y -= object.speed;
	if (object.move_to_down) 
		object.destination.y += object.speed;
}

void pointer_stop(int p_direction) {
	switch (p_direction)
	{
	case VK_LEFT:
		object.move_to_left = FALSE;
		break;
	case VK_RIGHT:
		object.move_to_right = FALSE;
		break;
	case VK_UP:
		object.move_to_up = FALSE;
		break;
	case VK_DOWN:
		object.move_to_down = FALSE;
		break;
	default:
		object.move_to_left = FALSE;
		object.move_to_right = FALSE;
		object.move_to_down = FALSE;
		object.move_to_up = FALSE;
		break;
	}
}

//TO_DO:...
void pointer_clutch() {
}

void pointer_draw(HDC hdc) {
	SetPixel(hdc, object.destination.x, object.destination.y, RGB(0, 0, 0));
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	HBRUSH TransperrantBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HGDIOBJ replaced_brush_handle = SelectObject(hdc, TransperrantBrush);
	HGDIOBJ replaced_pen_handle = SelectObject(hdc, hPen);
	Rectangle(hdc, object.destination.x - POINTERRADIUS, object.destination.y - POINTERRADIUS,
		object.destination.x + POINTERRADIUS, object.destination.y + POINTERRADIUS);
	MoveToEx(hdc, object.destination.x, object.destination.y - 2, NULL);
	LineTo(hdc, object.destination.x, object.destination.y - POINTERRADIUS);
	MoveToEx(hdc, object.destination.x, object.destination.y + 2, NULL);
	LineTo(hdc, object.destination.x, object.destination.y + POINTERRADIUS - 1);
	MoveToEx(hdc, object.destination.x - 2, object.destination.y, NULL);
	LineTo(hdc, object.destination.x - POINTERRADIUS, object.destination.y);
	MoveToEx(hdc, object.destination.x + 2, object.destination.y, NULL);
	LineTo(hdc, object.destination.x + POINTERRADIUS-1, object.destination.y);
	SelectObject(hdc, replaced_pen_handle);
	SelectObject(hdc, replaced_brush_handle);
	DeleteObject(hPen);	
}

boolean is_pointer(int pointer_h)
{
	if (pointer_h == id)
		return TRUE;
	else return FALSE;
}

struct _coo get_pointer_destination(int handler)
{
	return object.destination;
}

void set_pointer_destination(struct _coo destination) {
	object.destination = destination;
}

//TO-DO:...
void pointer_release(int h, POINT pt) {
}

void pointer_set_size(int ppointerh, struct _coo psize) {
	if (ppointerh != id) return;
	object.size = psize;
}

struct _coo pointer_get_size(int ppointerh) {
	struct _coo result = { .x = -1, .y = -1 };
	if (ppointerh == id)
		result = object.size;
	return result;
}