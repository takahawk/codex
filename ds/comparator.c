#include "comparator.h"

#include <stdint.h>
#include <string.h>

static int
_str_comparator(const void *lhs, const void *rhs, void *ctx) {
	return strcmp(lhs, rhs);
}

static int
_uint16_comparator(const void *lhs, const void *rhs, void *ctx) {
	uint16_t l = *((uint16_t *) lhs);
	uint16_t r = *((uint16_t *) rhs);
	if (l < r)
		return -1;
	else if (l > r)
		return 1;
	else
		return 0;
}

static int
_ptr_comparator(const void *lhs, const void *rhs, void *ctx) {
	if (lhs < rhs)
		return -1;
	else if (lhs > rhs)
		return 1;

	return 0;
}

const Comparator str_comparator = {
	.cb = _str_comparator,
	.ctx = NULL
};

const Comparator uint16_comparator = {
	.cb = _uint16_comparator,
	.ctx = NULL
};

const Comparator ptr_comparator = {
	.cb = _ptr_comparator,
	.ctx = NULL
};