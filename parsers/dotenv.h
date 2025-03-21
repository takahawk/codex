#ifndef PARSERS_DOTENV_H_
#define PARSERS_DOTENV_H_
#include <stdbool.h>
#include <stdint.h>

#include "allocators/allocator.h"
#include "../ds/array.h"

typedef struct Dotenv {
	// TODO: probably better to use trees... when we implement one in codex
	Array/*char**/ *keys;
	Array/*char**/ *values;

	const char* (*get_string) (struct Dotenv *self, const char *key);
	uint16_t    (*get_uint16) (struct Dotenv *self, const char *key);
	bool        (*get_bool)   (struct Dotenv *self, const char *key);

	void        (*release)    (struct Dotenv **pself);
} Dotenv;

Dotenv* parse_dotenv(Allocator *a, const char *buffer);
#endif