#include "codex/ds/rbtree.h"

#include <string.h>

static int
_default_comparator(const void *lhs, const void *rhs, void *ctx) {
	size_t key_size;
	memcpy(&key_size, &ctx, sizeof(key_size));
	return memcmp(lhs, rhs, key_size);

}

static RBTreeNode*
_rb_tree_node_get(RBTreeNode *root, void *key, RBComparator cmp) {
	if (root == NULL)
		return NULL;
	size_t diff = cmp.cb(key, root->key, cmp.ctx);
	if (diff == 0)
		return root;
	else if (diff < 0) {
		return _rb_tree_node_get(root->left, key, cmp);
	} else {
		return _rb_tree_node_get(root->right, key, cmp);
	}

}

static RBTreeNode*
rb_tree_get(RBTree *self, void *key) {
	return _rb_tree_node_get(self->head, key, self->comparator);
}

static bool 
rb_tree_remove(RBTree *self, void *key) {
	// TODO: impl
}

static void
rb_tree_add(RBTree *self, void *key, void *satellite) {
	// TODO: impl
}

static void
rb_tree_release(RBTree **pself) {
	RBTree *self = *pself;
	Allocator *a = self->a;

	// TODO: free nodes
	a->free(a, self);
	*pself = NULL;
}

static RBTree RB_TREE_PROTOTYPE = {
	.head   = NULL,

	.comparator = (RBComparator) {
		.cb = _default_comparator
	},
	.get    = rb_tree_get,
	.remove = rb_tree_remove,
	.add    = rb_tree_add,

	.release = rb_tree_release
};

RBTree* form_rb_tree(Allocator *a, size_t key_size) {
	RBTree *rb = a->alloc(a, sizeof(RBTree));

	*rb = RB_TREE_PROTOTYPE;
	rb->a = a;
	rb->key_size = key_size;

	memcpy(&rb->comparator.ctx, &key_size, sizeof(key_size));

	return rb;
}