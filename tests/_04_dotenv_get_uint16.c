#include <stdbool.h>
#include <string.h>
#include "codex/parsers/dotenv.h"
#include "codex/testing/assert.h"

#include "codex/mem/debug_allocator.h"


int main() {
	Allocator *a = form_debug_allocator(&STD_ALLOCATOR);
	DebugAllocatorCtx *allocCtx = a->ctx;
	char *buffer = "TheStar=1337\n"
				   "Feel=7331\n";
	Dotenv *dotenv = parse_dotenv(a, buffer);

	uint16_t value = dotenv->get_uint16(dotenv, "TheStar");
	assert_uint16_equals(value, 1337);

	value = dotenv->get_uint16(dotenv, "Feel");
	assert_uint16_equals(value, 7331);

	dotenv->release(&dotenv);

	if (allocCtx->allocations->len != 0) {
		allocCtx->print_allocations(allocCtx);
		return -1;
	}
	return 0;
}
