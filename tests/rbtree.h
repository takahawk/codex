#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "codex/allocators/allocator.h"
#include "codex/ds/array.h"
#include "codex/ds/rbtree.h"

static void
_calculate_black_heights(RBTreeNode *root, size_t current, Array *bhs) {
	if (NULL == root) {
		bhs->add(bhs, &current);
		return;
	}

	_calculate_black_heights(root->left,  current + !root->is_red, bhs);
	_calculate_black_heights(root->right, current + !root->is_red, bhs);
}

static void 
assert_valid_rbtree(RBTree *rb) {
	if (NULL == rb->root) {
		return;
	}
	if (rb->root != NULL && rb->root->is_red) {
		fprintf(stderr, "RB tree root is red (supposed to always stay black)\n");
		exit(EXIT_FAILURE);
	}

	Iterator *it = rb->form_iterator(rb);

	while (it->has_next(it)) {
		RBTreeNode *node = it->next(it);
		if (node->is_red) {
			if ((node->left && node->left->is_red) ||
				(node->right && node->right->is_red)) {
				fprintf(stderr, "there is red child of red node\n");
				exit(EXIT_FAILURE);
			}
		}
	}

	it->release(&it);

	Array/*size_t*/ *bhs = ARRAY.form(&STD_ALLOCATOR, sizeof(size_t));
	_calculate_black_heights(rb->root, 0, bhs);

	if (bhs->len != 0) {
		size_t val = *((size_t *) bhs->get(bhs, 0));

		for (size_t i = 1; i < bhs->len; ++i) {
			size_t val2 = *((size_t *) bhs->get(bhs, i));
			if (val != val2) {
				fprintf(stderr, "black heights are not the same for two or more paths\n");
				exit(EXIT_FAILURE);

			}
		}
	}
	bhs->release(&bhs);
}
