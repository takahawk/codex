#ifndef CODEX_TESTING_ASSERT_H_
#define CODEX_TESTING_ASSERT_H_
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static inline void
assert_str_equals(const char *actual, const char *expected) {
	if (strcmp(actual, expected) != 0) {
		fprintf(stderr, "assertion failed. actual=\"%s\" expected=\"%s\"\n", actual, expected);
		exit(EXIT_FAILURE);
	}
}

static inline void
assert_bool_equals(bool actual, bool expected) {
	if (actual != expected) {
		fprintf(stderr, "assertion failed. actual=\"%s\" expected=\"%s\"\n", 
			    actual ? "true" : "false", expected ? "true" : "false");
		exit(EXIT_FAILURE);
	}
}

static inline void
assert_uint16_equals(uint16_t actual, uint16_t expected) {
	if (actual != expected) {
		fprintf(stderr, "assertion failed. actual=%d expected=%d\n", actual, expected);
		exit(EXIT_FAILURE);
	}
}
#endif