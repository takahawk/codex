#include "array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_BASE_CAP 10
#define CAP_MULTIPLIER 2

static inline void
array_release(Array **pself) {
	if (NULL == *pself) {
		fprintf(stderr, "Trying to release NULL pointer (array)\n");
		exit(EXIT_FAILURE);
	}
	free((*pself)->data);
	free(*pself);
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
		self->data = realloc(self->data, self->elem_size * self->cap);
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

Array*
form_array(size_t elem_size) {
	Array *arr = malloc(sizeof(Array));
	arr->elem_size = elem_size;
	arr->len = 0;
	arr->cap = ARRAY_BASE_CAP;
	arr->data = malloc(elem_size * arr->cap);

	arr->add = array_add;
	arr->set = array_set;
	arr->get = array_get;
	arr->fremove = array_fast_remove;
	arr->release = array_release;

	return arr;
}
