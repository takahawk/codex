#include "iterator.h"

#include <stdlib.h>

typedef struct {
	Iterator   *orig_iter;
	void       *orig_ctx;
	FilterFunc ff;
	void       *ff_ctx;
	void       *next;
} FilterIterCtx;

#define PROLOGUE \
	FilterIterCtx *ctx = self->ctx; \
	Iterator *orig_iter = ctx->orig_iter; \
	void *orig_ctx = ctx->orig_ctx; \
	FilterFunc ff = ctx->ff; \
	void *ff_ctx = ctx->ff_ctx;


static void
find_next(Iterator *self) {
	PROLOGUE

	while (orig_iter->has_next(orig_iter)) {
		void *value = orig_iter->next(orig_iter);
		if (ff(value, ff_ctx)) {
			ctx->next = value;
			return;
		}
	}

	ctx->next = NULL;
}

static bool
filter_iterator_has_next(Iterator *self) {
	PROLOGUE
	return ctx->next == NULL;
}

static void*
filter_iterator_next(Iterator *self) {
	PROLOGUE
	void *value = ctx->next;
	find_next(self);
	return value;
}

static void
filter_iterator_release(Iterator **pself) {
	Iterator *self = *pself;
	PROLOGUE

	Allocator *a = orig_iter->a;
	orig_iter->release(&orig_iter);
	a->free(a, ctx);
	a->free(a, self);

	*pself = NULL;
}

Iterator* iterator_filter(Iterator *self, FilterFunc ff, void *ff_ctx) {
	Allocator *a = self->a;
	
	Iterator *filtered = a->alloc(a, sizeof(Iterator));
	*filtered = ITERATOR_PROTOTYPE;

	FilterIterCtx *ctx = a->alloc(a, sizeof(FilterIterCtx));
	ctx->orig_iter = self;
	ctx->orig_ctx = self->ctx;
	ctx->ff = ff;
	ctx->ff_ctx = ff_ctx;
	filtered->ctx = ctx;


	filtered->has_next = filter_iterator_has_next;
	filtered->next = filter_iterator_next;

	find_next(filtered);

	return filtered;
}

const Iterator ITERATOR_PROTOTYPE = {
	.filter = iterator_filter
};

