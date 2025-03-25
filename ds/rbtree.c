#include "codex/ds/rbtree.h"

#include <string.h>

static RBTreeNode*
_rb_tree_node_get(RBTreeNode *root, void *key, Comparator cmp) {
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

static void
_rb_tree_left_rotate(RBTree *self, RBTreeNode *x) {
	RBTreeNode *y = x->right;
	x->right = y->left;
	if (y->left != NULL) {
		y->left->p = x;
	}

	y->p = x->p;

	if (self->root == x) {
		self->root = y;
	} else if (x == x->p->left) {
		x->p->left = y;
	} else {
		x->p->right = y;
	}

	y->left = x;
	x->p = y;
}

static void
_rb_tree_right_rotate(RBTree *self, RBTreeNode *x) {
	RBTreeNode *y = x->left;
	x->left = y->right;
	if (NULL != y->right) {
		y->right->p = x;
	}

	y->p = x->p;

	if (self->root == x) {
		self->root = y;
	} else if (x == x->p->left) {
		x->p->left = y;
	} else {
		x->p->right = y;
	}

	y->right = x;
	x->p = y;
}

static RBTreeNode*
rb_tree_get(RBTree *self, void *key) {
	return _rb_tree_node_get(self->root, key, self->comparator);
}

static bool 
rb_tree_remove(RBTree *self, void *key) {
	// TODO: impl
}

static void
rb_tree_set(RBTree *self, void *key, void *value) {
	Allocator *a = self->a;
	Comparator cmp = self->comparator;


	RBTreeNode *node = self->root;
	RBTreeNode *p = NULL;

	while (node != NULL) {
		p = node;
		int c = cmp.cb(key, node->key, cmp.ctx);
		if (c == 0) {
			// TODO: free prev value?
			node->value = value;
			return;
		} else if (c < 0) {
			node = node->left;
		} else {
			node = node->right;
		}
	}

	RBTreeNode *newnode = a->alloc(a, sizeof(RBTreeNode));
	newnode->key = key;
	newnode->value = value;
	newnode->left = NULL;
	newnode->right = NULL;
	newnode->p = p;

	if (NULL == p) {
		self->root = newnode;
		newnode->is_red = false;
		return;
	}

	int c = cmp.cb(newnode->key, p, cmp.ctx);
	if (c > 0) {
		p->right = newnode;
	} else {
		p->left = newnode;
	}

	p->is_red = true;

	// TODO: fix the tree
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
	.root   = NULL,

	.get    = rb_tree_get,
	.remove = rb_tree_remove,
	.set    = rb_tree_set,

	.release = rb_tree_release
};

RBTree* form_rb_tree(Allocator *a, Comparator comparator) {
	RBTree *rb = a->alloc(a, sizeof(RBTree));

	*rb = RB_TREE_PROTOTYPE;
	rb->a = a;
	rb->comparator = comparator;

	return rb;
}