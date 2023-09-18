#include "utils.h"

#define BORDERSCNT 4
#define ANGLESCNT 4
#define MAXRESDIST 5.0f

typedef boolean(*bump_check_func) (struct _coo*, char*);

static struct _region {
	float y_top;
	float y_bottom;
	float x_left;
	float x_right;
};

boolean check_top_along_height(struct _coo* next_position, char* res_code);
boolean check_bottom_along_height(struct _coo* next_position, char* res_code);
boolean check_left_along_width(struct _coo* next_position, char* res_code);
boolean check_right_along_width(struct _coo* next_position, char* res_code);
struct _coo checker(char* pcode);

struct collision_check {
	bump_check_func borders[BORDERSCNT];
	struct _coo(*checker)(char*);
	struct _region region;
	struct _region oper_region;
	struct _coo potentionalNextPosition;
};

static struct collision_check bumpManager = {
	.borders = {
		check_top_along_height,
		check_bottom_along_height,
		check_left_along_width,
		check_right_along_width
	},
	.checker = checker
};

static boolean check_top_along_height(struct _coo* next_position, char* res_code) {
	boolean result = FALSE;
	if (next_position->y < bumpManager.oper_region.y_top) {
		result = TRUE;
		(*res_code) |= 0b0001;
		next_position->y = bumpManager.oper_region.y_top + fabs(next_position->y - bumpManager.oper_region.y_top);
		if ((next_position->y - bumpManager.oper_region.y_top) < MAXRESDIST)
			next_position->y = bumpManager.oper_region.y_top;
	}
	return result;
}

static boolean check_bottom_along_height(struct _coo* next_position, char* res_code) {
	boolean result = FALSE;
	if (next_position->y > bumpManager.oper_region.y_bottom) {
		result = TRUE;
		(*res_code) |= 0b0010;
		next_position->y = bumpManager.oper_region.y_bottom - fabs(next_position->y - bumpManager.oper_region.y_bottom);
		if ((bumpManager.oper_region.y_bottom - next_position->y) < MAXRESDIST)
			next_position->y = bumpManager.oper_region.y_bottom;
	}
	return result;
}

static boolean check_left_along_width(struct _coo* next_position, char* res_code) {
	boolean result = FALSE;
	if (next_position->x < bumpManager.oper_region.x_left) {
		result = TRUE;
		(*res_code) |= 0b0100;
		next_position->x = bumpManager.oper_region.x_left + fabs(next_position->x - bumpManager.oper_region.x_left);
		if ((next_position->x - bumpManager.oper_region.x_left) < MAXRESDIST)
			next_position->x = bumpManager.oper_region.x_left;
	}
	return result;
}

static boolean check_right_along_width(struct _coo* next_position, char* res_code) {
	boolean result = FALSE;
	if (next_position->x > bumpManager.oper_region.x_right) {
		result = TRUE;
		(*res_code) |= 0b1000;
		next_position->x = bumpManager.oper_region.x_right - fabs(next_position->x - bumpManager.oper_region.x_right);
		if ((bumpManager.oper_region.x_right - next_position->x) < MAXRESDIST)
			next_position->x = bumpManager.oper_region.x_right;
	}
	return result;
}

static struct _coo checker(char* pcode) {
	char result_code = 0;
	struct _coo result = bumpManager.potentionalNextPosition;
	char code = *pcode;
	int i = 0;
	while (i < BORDERSCNT) {
		if ((code >> i) & 0b0001) {
			bump_check_func func = bumpManager.borders[i];
			if (func(&result, &result_code)) {
				i = 0;
				continue;
			}
		}
		i++;
	}
	(*pcode) = result_code;
	return result;
}

void dump_set_region_borders(POINT pregionSize) {
	struct _coo coo = utilConvertWinPointToCoo(pregionSize);
	bumpManager.region.y_top = 0;
	bumpManager.region.y_bottom = coo.y;
	bumpManager.region.x_left = 0;
	bumpManager.region.x_right = coo.x;
}

static inline void dump_set_potential_next_position(struct _coo ppotentialNextPosition) {
	bumpManager.potentionalNextPosition = ppotentialNextPosition;
}

static struct _region edit_region_borders(struct _coo object_center_p) {
	struct _region result = bumpManager.region;
	result.y_top += object_center_p.y;
	result.y_bottom -= object_center_p.y;
	result.x_left += object_center_p.x;
	result.x_right -= object_center_p.x;
	return result;
}

static inline void set_oper_region(struct _region region) {
	bumpManager.oper_region = region;
}

struct _coo bump(struct _coo ppotentialNextPosition, struct _coo object_center_p, char* bumped_code) {
	struct _coo result = { 0 };
	dump_set_potential_next_position(ppotentialNextPosition);
	set_oper_region(edit_region_borders(object_center_p));
	result = bumpManager.checker(bumped_code);
	return result;
}

int util_random(int upper_limit) {
	HCRYPTPROV hCryptProv;
	BYTE randomBytes[4];

	if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
		return 1;
	}

	// Generate 4 random bytes
	if (!CryptGenRandom(hCryptProv, sizeof(randomBytes), randomBytes)) {
		CryptReleaseContext(hCryptProv, 0);
		return 1;
	}

	// Convert the random bytes to an integer
	int randomNumber = (randomBytes[0] << 24) | (randomBytes[1] << 16) | (randomBytes[2] << 8) | randomBytes[3];

	randomNumber %= upper_limit;

	// Release the cryptographic service provider handle
	CryptReleaseContext(hCryptProv, 0);

	return randomNumber;
}