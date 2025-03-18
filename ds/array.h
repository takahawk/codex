#ifndef ARRAY_H_
#define ARRAY_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct Array {
	uint8_t *data;
	size_t len;
	size_t cap;
	size_t elem_size;

	// add value to array. pval is an address of value to be copied to the array
	void  (*add)     (struct Array *self, void *pval);
	void* (*get)     (struct Array *self, size_t i);
	// sets value of array. value on pval address will be copied
	void  (*set)     (struct Array *self, size_t i, void *pval);
	// fast remove without preserving order - element is just replaced by last one
	void  (*fremove) (struct Array *self, size_t i);

	bool  (*equals)  (struct Array *self, struct Array *other);

	void  (*sort)    (struct Array *self, int (*compar) (const void*, const void*));

	void  (*release) (struct Array **pself);
} Array;

Array* form_array(size_t elem_size);

#endif