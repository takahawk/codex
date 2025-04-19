#ifndef CODEX_RBTREE_H_
#define CODEX_RBTREE_H_
#include "codex/mem/allocator.h"
#include "codex/ds/comparator.h"
#include "codex/ds/iterator.h"

#include "stdbool.h"

typedef struct RBTreeNode RBTreeNode;
typedef struct RBTreeNode {
	bool is_red;
	void *key;
	void *value;

	RBTreeNode *left;
	RBTreeNode *right;
	RBTreeNode *p;
} RBTreeNode;

typedef struct RBTree RBTree;
typedef struct RBTree {
	Allocator  *a;
	RBTreeNode *root;
	Comparator comparator;

	void        (*set)           (RBTree *self, void *key, void *value);
	RBTreeNode* (*get)           (RBTree *self, void *key);
	bool        (*remove)        (RBTree *self, void *key);
	Iterator*   (*form_iterator) (RBTree *self);

	void        (*release)       (RBTree **pself);
} RBTree;

RBTree *form_rbtree(Allocator *a, Comparator comparator);
#endif
