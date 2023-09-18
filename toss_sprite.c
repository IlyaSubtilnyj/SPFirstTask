#define TOSSSPRITEIMPL
#include "toss_sprite.h"
#undef TOSSSPRITEIMPL

struct mdlInfo {
	boolean initialized;
	char* version;
};

static HWND ghWnd;
static struct mdlInfo mdl = { .initialized = FALSE, .version = NULL };

void mdlInit(char* version) {
	mdl.initialized = TRUE;
	mdl.version = version;
}

void mdlDestroy() {
	mdl.initialized = FALSE;
	mdl.version = NULL;
}

void init(HWND phWnd, char* version) {
	assert(phWnd && "pHWnd is NULL");
	ghWnd = phWnd;
	mdlInit(version);
}

void destroy(HWND phWnd) {
	assert((phWnd == ghWnd) && "Incorrect window handler");
	mdlDestroy();
	sprite_destroy();
}

//TO-DO:...
void bindSpToP(int pointer_h, int sprite_h) {
	if (!(is_pointer(pointer_h) && is_sprite(sprite_h))) return;
	
}

POINT getRegionSize() {
	POINT result;
	RECT rect;
	GetWindowRect(ghWnd, &rect);
	result.x = rect.right - rect.left;
	result.y = rect.bottom - rect.top;
	int captionHeight = GetSystemMetrics(SM_CYCAPTION);
	int frameHeight = GetSystemMetrics(SM_CYFRAME);
	int borderHeight = GetSystemMetrics(SM_CYBORDER);
	int panelHeight = captionHeight + 2 * frameHeight + 6 * borderHeight;
	int panelWidth = 2 * frameHeight + 6 * borderHeight;
	result.x -= (float)panelWidth;
	result.y -= (float)panelHeight;
	return result;
}

void sprite_move_wrapper(int pointer_h, float time) {
	char bumped_code = 0b1111;
	dump_set_region_borders(getRegionSize());
	struct _coo destination = get_pointer_destination(pointer_h);
	sprite_move(destination, time, &bumped_code);
	struct _coo lpointerSize = { .x = 73, .y = 93 };
	struct _coo new_pointer_destination = bump(destination, lpointerSize, &bumped_code);
	set_pointer_destination(new_pointer_destination);
}

void pointer_clutch_wrapper(int pointer_h, POINT point) {
	int sprite_h = 1;
	struct _coo coo;
	if ((point.x == -1) && (point.y == -1)) {
		point = getRegionSize();
		coo.x = util_random(point.x);
		coo.y = util_random(point.y);
	}
	else {
		coo.x = (float)point.x;
		coo.y = (float)point.y;
	}
	set_pointer_destination(coo);
}

void pointer_release_wrapper(int h, POINT pt) {
	struct _coo destination = get_pointer_destination(h);
	struct _coo dest;
	dest.x = pt.x;
	dest.y = pt.y;
	struct _coo dif = cooSub(dest, destination);
	dif = cooAdd(destination, cooMul(dif, 19));
	set_pointer_destination(dif);
}

int sprite_create_wrapper(wchar_t* pBitmapName, POINT pdropCoordinates) {
	if (!mdl.initialized) return 1;
	if (sprite_create(pBitmapName, pdropCoordinates)) {
		MessageBox(ghWnd, L"Failed to load image", L"Error", MB_OK);
	}
}

struct _toss_sprite_namespace mdltosssprite = {
	.sprite = {
		.create = sprite_create_wrapper,
		.move = sprite_move_wrapper,
		.draw = sprite_draw
	},
	.pointer = {
		.create = pointer_create,
		.get_moving = pointer_get_moving,
		.move = pointer_move,
		.stop = pointer_stop,
		.clutch = pointer_clutch_wrapper,
		.release = pointer_release_wrapper,
		.draw = pointer_draw
	},
	.bind_sprite_to_pointer = bindSpToP,
	.init = init,
	.destroy = destroy
};