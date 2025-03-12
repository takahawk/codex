#ifndef ARRAY_H_
#define ARRAY_H_

#include <stdio.h>
#include <stdlib.h>

#define ARRAY_BASE_CAP 10
#define CAP_MULTIPLIER 2

typedef struct Array {
	uint8_t *data;
	size_t len;
	size_t cap;
	size_t elem_size;

	// add value to array. pval is an address of value to be copied to the array
	void  (*add) (struct Array *self, void *pval);
	void* (*get) (struct Array *self, size_t i);
	// sets value of array. value on pval address will be copied
	void  (*set) (struct Array *self, size_t i, void *pval);

	void  (*release) (struct Array **pself);
} Array;

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

static inline void
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

static inline void*
array_get(Array *self, size_t i) {
	if (i >= self->len) {
		printf("index out of bounds (i=%ld array.len=%ld)\n", i, self->len);
		exit(EXIT_FAILURE);
	}
	return self->data + self->elem_size * i;
}

static Array*
form_array(size_t elem_size) {
	Array *arr = malloc(sizeof(Array));
	arr->elem_size = elem_size;
	arr->len = 0;
	arr->cap = ARRAY_BASE_CAP;
	arr->data = malloc(elem_size * arr->cap);

	arr->add = array_add;
	arr->set = array_set;
	arr->get = array_get;
	arr->release = array_release;

	return arr;
}

#endif