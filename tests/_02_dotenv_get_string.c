#include <string.h>

#include "codex/allocators/allocator.h"
#include "codex/allocators/debug_allocator.h"
#include "codex/parsers/dotenv.h"
#include "codex/testing/assert.h"


int main() {
	Allocator *a = form_debug_allocator(&STD_ALLOCATOR);
	DebugAllocatorCtx *allocCtx = a->ctx;
	char *buffer = "Zero=TheOne\n"
				   "Architect=Galatea\n"
				   "Simulation=Emulation";
	Dotenv *dotenv = parse_dotenv(a, buffer);

	const char *val = dotenv->get_string(dotenv, "Zero");
	assert_str_equals(val, "TheOne");

	val = dotenv->get_string(dotenv, "Architect");
	assert_str_equals(val, "Galatea");

	val = dotenv->get_string(dotenv, "Simulation");
	assert_str_equals(val, "Emulation");

	dotenv->release(&dotenv);
	if (allocCtx->allocations->len != 0) {
		allocCtx->print_allocations(allocCtx);
		return -1;
	}

	return 0;
}
