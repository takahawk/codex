#include "codex/ds/rbtree.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	Allocator           *a;
	RBTreeNode          *node;
} RBTreeIteratorCtx;

static bool
rbtree_iterator_has_next(Iterator *self) {
	RBTreeIteratorCtx *ctx = self->ctx;
	return NULL != ctx->node;
}

static void
find_next(Iterator *self) {
	RBTreeIteratorCtx *ctx = self->ctx;
	RBTreeNode *node = ctx->node;

	if (node == NULL) {
		fprintf(stderr, "there is no next element\n");
		exit(EXIT_FAILURE);
	}

	if (node->right) {
		node = node->right;
		while (node->left) {
			node = node->left;
		}
		ctx->node = node;
		return;
	} 

	while (node->p != NULL && node->p->right == node) {
		node = node->p;
	}

	if (NULL == node->p) {
		ctx->node = NULL;
		return;
	}

	// node->p->left == node
	ctx->node = node->p;
	return;

}

static void*
rbtree_iterator_next(Iterator *self) {
	RBTreeIteratorCtx *ctx = self->ctx;
	RBTreeNode *node = ctx->node;
	if (NULL == node) {
		fprintf(stderr, "there is no next element\n");
		exit(EXIT_FAILURE);
	}

	find_next(self);
	return node;
}

static void
rbtree_iterator_release(Iterator **pself) {
	Iterator *self = *pself;
	RBTreeIteratorCtx *ctx = self->ctx;
	Allocator *a = ctx->a;
	a->free(a, self->ctx);
	a->free(a, self);

	*pself = NULL;
}

const Iterator RBTREE_ITER_PROTOTYPE = {
	.has_next = rbtree_iterator_has_next,
	.next = rbtree_iterator_next,
	.release = rbtree_iterator_release
};

static Iterator*
rb_tree_form_iterator(RBTree *self) {
	Allocator *a = self->a;

	RBTreeIteratorCtx *ctx = a->alloc(a, sizeof(RBTreeIteratorCtx));
	ctx->a = a;
	ctx->node = self->root;
	if (NULL != ctx->node) {
		while (ctx->node->left)
			ctx->node = ctx->node->left;
	}

	Iterator *i = a->alloc(a, sizeof(Iterator));
	*i = RBTREE_ITER_PROTOTYPE;
	i->ctx = ctx;

	return i;
}