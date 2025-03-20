#include "ds/array.h"

#include <stdbool.h>
#include <string.h>

#include "testing/assert.h"

bool is_item_release_called[5];

static void
item_release(int **pitem) {
	int number = **pitem;
	is_item_release_called[number] = true;
}

int main() {
	bzero(is_item_release_called, sizeof(bool) * 5);
	Array *arr = form_array(sizeof(int));
	int x1 = 0, x2 = 1, x3 = 2, x4 = 3, x5 = 4;
	arr->add(arr, &x1);
	arr->add(arr, &x2);
	arr->add(arr, &x3);
	arr->add(arr, &x4);
	arr->add(arr, &x5);

	arr->item_release = (ArrayItemReleaseCb) item_release;

	arr->release(&arr);

	for (size_t i = 0; i < 5; ++i) {
		assert_bool_equals(is_item_release_called[i], true);
	}

	return 0;
}
