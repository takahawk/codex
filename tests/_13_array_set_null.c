#include "codex/ds/array.h"

#include <stdbool.h>
#include <string.h>

#include "codex/allocators/debug_allocator.h"
#include "codex/testing/assert.h"

int main() {
	Allocator *a = form_debug_allocator(&std_allocator);
	DebugAllocatorCtx *allocCtx = a->ctx;
	Array *arr = ARRAY.form(a, sizeof(int));
	int x1 = 0, x2 = 1, x3 = 2, x4 = 3, x5 = 4;
	arr->add(arr, &x1);
	arr->add(arr, &x2);
	arr->add(arr, &x3);
	arr->add(arr, &x4);
	arr->add(arr, NULL);



	for (size_t i = 0; i < 4; ++i) {
    int *val = arr->get(arr, i);
    assert_int_equals(*val, i);
	}
  int *nl = arr->get(arr, 4);
  assert_int_equals(*nl, 0);

	arr->release(&arr);
	if (allocCtx->allocations->len != 0) {
		allocCtx->print_allocations(allocCtx);
		return -1;
	}

	return 0;
}
