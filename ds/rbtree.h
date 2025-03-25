#ifndef CODEX_RBTREE_H_
#define CODEX_RBTREE_H_
#include "codex/allocators/allocator.h"

#include "stdbool.h"

typedef struct RBTreeNode RBTreeNode;
typedef struct RBTreeNode {
	void *key;
	void *satellite;

	RBTreeNode *left;
	RBTreeNode *right;
} RBTreeNode;

typedef struct RBTree RBTree;
typedef struct RBTree {
	Allocator  *a;
	RBTreeNode *head;
	// TODO: custom comparator?

	RBTreeNode* (*get)    (struct RBTree *self, void *key);
	bool        (*remove) (struct RBTree *self, void *key);
	void        (*add)    (struct RBTree *self, void *key, void *satellite);

	void        (*release) (struct RBTree **pself);
} RBTree;

RBTree *form_rbtree(Allocator *a);
#endif