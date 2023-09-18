#pragma once

#include <windows.h>
#include <math.h>

struct _coo {
	float x;
	float y;
};

struct _coo bump(struct _coo ppotentialNextPosition, struct _coo pobjectCenter, char* bumped_code);
void dump_set_region_borders(POINT pregionSize);
int util_random(int upper_limit);

inline struct _coo lerp(struct _coo start, struct _coo end, float t) {
	struct _coo result = { 0 };
	start.x *= (1.0f - t);
	start.y *= (1.0f - t);
	end.x *= t;
	end.y *= t;
	result.x = start.x + end.x;
	result.y = start.y + end.y;
	return result;
}

inline struct _coo cooAdd(struct _coo first, struct _coo second) {
	struct _coo result = { 0 };
	result.x = first.x + second.x;
	result.y = first.y + second.y;
	return result;
}

inline struct _coo cooSub(struct _coo first, struct _coo second) {
	struct _coo result = first;
	result.x -= second.x;
	result.y -= second.y;
	return result;
}

inline struct _coo cooMul(struct _coo first, int m) {
	struct _coo result = first;
	result.x *= m;
	result.y *= m;
	return result;
}

inline boolean cooEquals(struct _coo first, struct _coo second) {
	boolean result = FALSE;
	if ((first.x == second.x) && (first.y == second.y)) result = TRUE;
	return result;
}

inline struct _coo utilConvertWinPointToCoo(POINT pwinPoint) {
	struct _coo result;
	result.x = (float)pwinPoint.x;
	result.y = (float)pwinPoint.y;
	return result;
}