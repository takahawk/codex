#ifndef CODEX_RBTREE_H_
#define CODEX_RBTREE_H_
#include "codex/allocators/allocator.h"

#include "stdbool.h"

typedef struct {
	int (*cb) (const void *, const void*, void *ctx);
	void *ctx;
} RBComparator;

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
	size_t     key_size;
	RBComparator comparator;

	void        (*add)    (RBTree *self, void *key, void *satellite);
	RBTreeNode* (*get)    (RBTree *self, void *key);
	bool        (*remove) (RBTree *self, void *key);

	void        (*release) (RBTree **pself);
} RBTree;

RBTree *form_rbtree(Allocator *a, size_t key_size);
#endif