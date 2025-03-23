#include <stdbool.h>
#include <string.h>

#include "codex/allocators/allocator.h"
#include "codex/allocators/debug_allocator.h"
#include "codex/parsers/dotenv.h"
#include "codex/testing/assert.h"


int main() {
	Allocator *a = form_debug_allocator(&std_allocator);
	DebugAllocatorCtx *allocCtx = a->ctx;
	char *buffer = "Existence=true\n"
				   "NonExistence=false\n";
	Dotenv *dotenv = parse_dotenv(a, buffer);

	bool value = dotenv->get_bool(dotenv, "Existence");
	assert_bool_equals(value, true);

	value = dotenv->get_bool(dotenv, "NonExistence");
	assert_bool_equals(value, false);

	dotenv->release(&dotenv);
	if (allocCtx->allocations->len != 0) {
		allocCtx->print_allocations(allocCtx);
		return -1;
	}
	return 0;
}