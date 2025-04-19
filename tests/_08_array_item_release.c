#include "codex/ds/array.h"

#include <stdbool.h>
#include <string.h>

#include "codex/mem/debug_allocator.h"
#include "codex/testing/assert.h"

bool is_item_release_called[5];

static void
item_release(int **pitem) {
	int number = **pitem;
	is_item_release_called[number] = true;
}

int main() {
	Allocator *a = form_debug_allocator(&STD_ALLOCATOR);
	DebugAllocatorCtx *allocCtx = a->ctx;
	bzero(is_item_release_called, sizeof(bool) * 5);
	Array *arr = ARRAY.form(a, sizeof(int));
	arr->item_release = (ArrayItemReleaseCb) item_release;
	int x1 = 0, x2 = 1, x3 = 2, x4 = 3, x5 = 4;
	arr->add(arr, &x1);
	arr->add(arr, &x2);
	arr->add(arr, &x3);
	arr->add(arr, &x4);
	arr->add(arr, &x5);


	arr->release(&arr);

	for (size_t i = 0; i < 5; ++i) {
		assert_bool_equals(is_item_release_called[i], true);
	}


	if (allocCtx->allocations->len != 0) {
		allocCtx->print_allocations(allocCtx);
		return -1;
	}

	return 0;
}
