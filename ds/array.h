#ifndef ARRAY_H_
#define ARRAY_H_

#include <stdint.h>
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
	void  (*add)     (struct Array *self, void *pval);
	void* (*get)     (struct Array *self, size_t i);
	// sets value of array. value on pval address will be copied
	void  (*set)     (struct Array *self, size_t i, void *pval);
	// fast remove without preserving order - element is just replaced by last one
	void  (*fremove) (struct Array *self, size_t i);

	void  (*release) (struct Array **pself);
} Array;

Array* form_array(size_t elem_size);

#endif