#include "codex/ds/array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ARRAY_BASE_CAP 10
#define CAP_MULTIPLIER 2

#define THEONE 1

const ArrayItemReleaseCb JUST_FREE_IT = (ArrayItemReleaseCb) THEONE;

static inline void
array_release(Array **pself) {
	Array *self = *pself;
	Allocator *a = self->a;
	if (NULL == self) {
		fprintf(stderr, "Trying to release NULL pointer (array)\n");
		exit(EXIT_FAILURE);
	}

	if (NULL != self->item_release) {
		size_t len = self->len;
		size_t elem_size = self->elem_size;
		void *p = self->data;

		void *maxp = p + (len * elem_size);
		for (; p < maxp; p += elem_size) {
			void *item = p;
			if (JUST_FREE_IT == self->item_release) {
				a->free(a, *(void **) item);
			} else {
				self->item_release(&item);
			}
		}
	}
	a->free(a, (*pself)->data);
	a->free(a, *pself);
	*pself = NULL;
}

static void
array_set(Array *self, size_t i, void *pval) {
	if (i >= self->len) {
		fprintf(stderr, "index out of bounds (i=%ld array.len=%ld)\n", i, self->len);
		exit(EXIT_FAILURE);
	}

	memcpy(self->data + (i * self->elem_size), pval, self->elem_size);
}

static void
array_add(Array *self, void *pval) {
	if (self->len == self->cap) {
		self->cap *= CAP_MULTIPLIER;
		self->data = self->a->realloc(self->a, self->data, self->elem_size * self->cap);
	}

	self->len++;
	array_set(self, self->len - 1, pval);
}

static void*
array_get(Array *self, size_t i) {
	if (i >= self->len) {
		printf("index out of bounds (i=%ld array.len=%ld)\n", i, self->len);
		exit(EXIT_FAILURE);
	}
	return self->data + self->elem_size * i;
}

static void
array_fast_remove(Array *self, size_t i) {
	if (i >= self->len) {
		fprintf(stderr, "index out of bounds (i=%ld array.len=%ld)\n", i, self->len);
		exit(EXIT_FAILURE);
	}

	if (self->len != 1) {
		memcpy(self->data + (i * self->elem_size),
			   self->data + ((self->len - 1) * self->elem_size),
			   self->elem_size);
	}

	self->len--;
}

static bool
array_equals(Array *self, Array *other) {
	if (self->elem_size != other->elem_size)
		return false;
	if (self->len != other->len)
		return false;

	return memcmp(self->data, other->data, self->elem_size * self->len) == 0;
}

static void
array_sort(Array* self, int (*compar) (const void *, const void *)) {
	qsort(self->data, self->len, self->elem_size, compar);
}

#include "array_serializer_internal.h"
const Array ARRAY_PROTOTYPE = {
	.item_release = NULL,

	.add = array_add,
	.set = array_set,
	.get = array_get,
	.fremove = array_fast_remove,
	.equals = array_equals,
	.sort = array_sort,
	.release = array_release,
	.form_serializer = form_array_serializer
};

Array*
form_array(Allocator *a, size_t elem_size) {
	Array *arr = a->alloc(a, sizeof(Array));
	*arr = ARRAY_PROTOTYPE;

	arr->a = a;
	arr->elem_size = elem_size;
	arr->len = 0;
	arr->cap = ARRAY_BASE_CAP;
	arr->data = a->alloc(a, elem_size * arr->cap);
	
	return arr;
}
