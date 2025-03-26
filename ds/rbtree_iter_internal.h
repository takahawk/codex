#include "codex/ds/rbtree.h"

#include <stdio.h>
#include <stdlib.h>

typedef enum {
	NOT_VISITED    = 0,
	LEFT_VISITED   = 1,
	COMPLETED      = 2
} RBTreeIteratorState;

typedef struct {
	Allocator           *a;
	RBTreeIteratorState state;
	RBTreeNode          *node;
} RBTreeIteratorCtx;

static bool
rbtree_iterator_has_next(Iterator *self) {
	RBTreeIteratorCtx *ctx = self->ctx;
	RBTreeIteratorState state = ctx->state;
	return state != COMPLETED;
}

static void*
rbtree_iterator_next(Iterator *self) {
	RBTreeIteratorCtx *ctx = self->ctx;
	RBTreeIteratorState state = ctx->state;
	RBTreeNode *node = ctx->node;
	if (state == COMPLETED) {
		fprintf(stderr, "there is no next element\n");
		exit(EXIT_FAILURE);
	}

	if (state == NOT_VISITED) {
		while (NULL != node->left) {
			node = node->left;
		}

		ctx->node = node->p;
		ctx->state = LEFT_VISITED;
		return node;
	}

	if (state == LEFT_VISITED) {
		RBTreeNode *returnNode = node;
		// find next at once
		if (NULL == node->right) {
			while (NULL != node->p && node->p->right == node) {
				node = node->p;
			}
			if (NULL == node->p) {
				ctx->state = COMPLETED;
			} else {
				// node was left child
				ctx->node = node;
				ctx->state = LEFT_VISITED;
			}
		} else {
			state = NOT_VISITED;
			ctx->node = node->right;
		}
		return returnNode;
	}

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
	ctx->state = self->root == NULL ? COMPLETED : NOT_VISITED;
	ctx->node = self->root;

	Iterator *i = a->alloc(a, sizeof(Iterator));
	*i = RBTREE_ITER_PROTOTYPE;
	i->ctx = ctx;

	return i;
}