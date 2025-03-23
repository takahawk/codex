#include <string.h>
#include "codex/allocators/allocator.h"
#include "codex/allocators/debug_allocator.h"
#include "codex/parsers/dotenv.h"
#include "codex/testing/assert.h"


int main() {
	Allocator *a = form_debug_allocator(&std_allocator);
	DebugAllocatorCtx *allocCtx = a->ctx;
	char *buffer = "Zero=TheOne\n"
				   "Architect=Galatea\n"
				   "Simulation=Emulation";
	Dotenv *dotenv = parse_dotenv(a, buffer);

	Array *keys = dotenv->keys;
	Array *values = dotenv->values;

	char *key = *(char **) keys->get(keys, 0);
	assert_str_equals(key, "Zero");

	key = *(char **) keys->get(keys, 1);
	assert_str_equals(key, "Architect");

	key = *(char **) keys->get(keys, 2);
	assert_str_equals(key, "Simulation");

	char *value = *(char **) values->get(values, 0);
	assert_str_equals(value, "TheOne");

	value = *(char **) values->get(values, 1);
	assert_str_equals(value, "Galatea");

	value = *(char **) values->get(values, 2);
	assert_str_equals(value, "Emulation");

	dotenv->release(&dotenv);

	if (allocCtx->allocations->len != 0) {
		allocCtx->print_allocations(allocCtx);
		return -1;
	}

	return 0;
}