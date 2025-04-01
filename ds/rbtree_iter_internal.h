#include "codex/ds/rbtree.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct {
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
	Allocator *a = self->a;
	a->free(a, self->ctx);
	a->free(a, self);

	*pself = NULL;
}

static Iterator*
rb_tree_form_iterator(RBTree *self) {
	Allocator *a = self->a;

	RBTreeIteratorCtx *ctx = a->alloc(a, sizeof(RBTreeIteratorCtx));
	ctx->node = self->root;
	if (NULL != ctx->node) {
		while (ctx->node->left)
			ctx->node = ctx->node->left;
	}

	Iterator *i = a->alloc(a, sizeof(Iterator));
	*i = ITERATOR_PROTOTYPE;
	i->has_next = rbtree_iterator_has_next;
	i->next = rbtree_iterator_next;
	i->release = rbtree_iterator_release;
	i->ctx = ctx;
	i->a = a;

	return i;
}