#ifndef CODEX_TESTING_ASSERT_H_
#define CODEX_TESTING_ASSERT_H_
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
#endif