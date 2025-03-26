#include "codex/ds/rbtree.h"

#include <string.h>

static RBTreeNode*
_rb_tree_node_get(RBTreeNode *root, void *key, Comparator cmp) {
	if (root == NULL)
		return NULL;
	int diff = cmp.cb(key, root->key, cmp.ctx);
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
	return _rb_tree_node_get(self->root, key, self->comparator);
}

static bool 
rb_tree_remove(RBTree *self, void *key) {
	// TODO: impl
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
		// root is always black
		newnode->is_red = false;
		return;
	}

	int c = cmp.cb(newnode->key, p->key, cmp.ctx);
	if (c > 0) {
		p->right = newnode;
	} else {
		p->left = newnode;
	}

	newnode->is_red = true;

	// fixing the tree now
	node = newnode;

	while (p->is_red) {
		// if it is red, then it MUST have a parent, because root is always black
		RBTreeNode *gp = p->p;

		// either...
		if (p == gp->left) {
			// ...we have uncle on the right (or NULL which counts as black)
			RBTreeNode *u = gp->right;

			if (NULL != u && u->is_red) {
				// "case 1": same color as of parent: recoloring and continuing
				//           loop from grandfather
				p->is_red = u->is_red = false;
				gp->is_red = true;
				node = gp;
				continue;
			} else {
				// different colors between uncle and parent
				if (node == p->right) {
					// "case 2": our node on the right, rotate parent left
					node = p;
					_rb_tree_left_rotate(self, node);
					p = node->p;
					gp = p->p;
					// FALLTHROUGH
				}

				// "case 3": our node on the left (either originally or parent
				//           from previous if clause). Change color of p and gp
				//           and rotate them
				p->is_red = false;
				gp->is_red = true;
				_rb_tree_right_rotate(self, p);
				continue;
			}
		} else {
			// ...or we have uncle on the left (or NULL which counts as black)
			RBTreeNode *u = gp->left;

			if (NULL != u && u->is_red) {
				// "case 1": same color as of parent: recoloring and continuing
				//           loop from grandfather
				p->is_red = u->is_red = false;
				gp->is_red = true;
				node = gp;
				continue;
			} else {
				// different colors between uncle and parent
				if (node == p->left) {
					// "case 2": our node on the right, rotate parent left
					node = p;
					_rb_tree_right_rotate(self, node);
					p = node->p;
					gp = p->p;
					// FALLTHROUGH
				}

				// "case 3": our node on the left (either originally or parent
				//           from previous if clause). Change color of p and gp
				//           and rotate them
				p->is_red = false;
				gp->is_red = true;
				_rb_tree_left_rotate(self, p);
				continue;
			}
		}
	}

	self->root->is_red = false;
}

static void
rb_tree_release(RBTree **pself) {
	RBTree *self = *pself;
	Allocator *a = self->a;

	// TODO: free nodes
	a->free(a, self);
	*pself = NULL;
}

#include "codex/ds/rbtree_iter_internal.h"

static RBTree RB_TREE_PROTOTYPE = {
	.root   = NULL,

	.get           = rb_tree_get,
	.remove        = rb_tree_remove,
	.set           = rb_tree_set,
	.form_iterator = rb_tree_form_iterator,

	.release = rb_tree_release
};

RBTree* form_rbtree(Allocator *a, Comparator comparator) {
	RBTree *rb = a->alloc(a, sizeof(RBTree));

	*rb = RB_TREE_PROTOTYPE;
	rb->a = a;
	rb->comparator = comparator;

	return rb;
}