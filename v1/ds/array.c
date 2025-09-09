#include "codex/v1/ds/array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ARRAY_BASE_CAP 10
#define CAP_MULTIPLIER 2

static inline void release(Array **pself) 
{
	Array *self = *pself;
	Allocator *a = self->a;
	if (NULL == self) {
		fprintf(stderr, "Trying to release NULL pointer (array)\n");
		exit(EXIT_FAILURE);
	}

  size_t len = self->len;
  size_t elem_size = self->elem_size;
  void *p = self->data;

  void *maxp = p + (len * elem_size);
  for (; p < maxp; p += elem_size) {
    self->release_cb.cb(self->release_cb.a, p);
  }

	a->free(a, self->data);
	a->free(a, self);
	*pself = NULL;
}

static void set(Array *self, size_t i, void *pval)
{
	if (i >= self->len) {
		fprintf(stderr, "index out of bounds (i=%ld array.len=%ld)\n", i, self->len);
		exit(EXIT_FAILURE);
	}

  void *ptr = self->data + (i * self->elem_size);
  if (!pval)
    bzero(ptr, self->elem_size);
  else
    memcpy(ptr, pval, self->elem_size);
}

static void add(Array *self, void *pval)
{
	if (self->len == self->cap) {
		self->cap *= CAP_MULTIPLIER;
		self->data = self->a->realloc(self->a, self->data, self->elem_size * self->cap);
	}

	self->len++;
	set(self, self->len - 1, pval);
}

static void* get(Array *self, size_t i) 
{
	if (i >= self->len) {
		printf("index out of bounds (i=%ld array.len=%ld)\n", i, self->len);
		exit(EXIT_FAILURE);
	}
	return self->data + self->elem_size * i;
}

static void fast_remove(Array *self, size_t i) 
{
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

static bool equals(Array *self, Array *other) 
{
	if (self->elem_size != other->elem_size)
		return false;
	if (self->len != other->len)
		return false;

	return memcmp(self->data, other->data, self->elem_size * self->len) == 0;
}

static void sort(Array* self, int (*compar) (const void *, const void *)) 
{
	qsort(self->data, self->len, self->elem_size, compar);
}

#include "array_serializer_internal.h"

Array* form_with_allocator(Allocator *a, size_t elem_size) 
{
	Array *arr = a->alloc(a, sizeof(Array));
	*arr = ARRAY.prototype;

	arr->a = a;
	arr->elem_size = elem_size;
	arr->len = 0;
	arr->cap = ARRAY_BASE_CAP;
	arr->data = a->alloc(a, elem_size * arr->cap);

	return arr;
}

Array* form(size_t elem_size) 
{
	return form_with_allocator(&STD_ALLOCATOR, elem_size);
}

Array* copy(Array *original)
{
  Allocator *a = original->a;
  Array *arr = a->alloc(a, sizeof(Array));

  arr->a         = a;
  arr->elem_size = original->elem_size;
  arr->len       = original->len;
  arr->cap       = original->cap;

  arr->data = a->alloc(a, arr->elem_size * arr->cap);
  memcpy(arr->data, original->data, arr->len * arr->elem_size);

  return arr;
}

const struct _ArrayStatic ARRAY = {
  .prototype = {

    .add = add,
    .set = set,
    .get = get,
    .fremove = fast_remove,
    .equals = equals,
    .sort = sort,
    .release = release,
    .form_serializer = form_array_serializer,

    .release_cb = RELEASE_CB.nop,
  },

  .form = form,
  .form_with_allocator = form_with_allocator,
  .copy = copy,
};

