#include "codex/ds/list.h"

static void 
prepend(List *self, void *value) {
	Allocator *a = self->a;
	ListNode *head = a->alloc(a, sizeof(ListNode));
	head->value = value;
	head->next = self->head;

	self->head = head;
}

static void 
append(List *self, void *value) {
	Allocator *a = self->a;
	ListNode *tail = self->tail(self);

	ListNode *node = a->alloc(a, sizeof(ListNode));
	node->value = value;
	tail->next = node;
}

static void 
remove (List *self, void *value) {
	Allocator *a = self->a;
	Comparator *cmp = self->cmp;
	if (NULL == self->head) {
		return;
	}

	ListNode *node = self->head;
	if (cmp->cb(self->head->value, value, cmp->ctx) == 0) {
		self->head = self->head->next;
		a->free(a, node);
		return;
	}
	while (NULL != node->next) {
		if (cmp->cb(node->next->value, value, cmp->ctx) == 0) {
			ListNode *tmp = node->next;
			node->next = node->next->next;
			a->free(a, tmp);
			return;
		}

		node = node->next;
	}
}

static ListNode* 
find(List *self, void *value) {
	Comparator *cmp = self->cmp;

	ListNode *node = self->head;

	while (NULL != node) {
		if (cmp->cb(node->value, value, cmp->ctx) == 0) {
			return node;
		}

		node = node->next;
	}

	return NULL;
}

static ListNode*
tail(List *self) {
	ListNode *node = self->head;

	if (NULL == node)
		return NULL;

	while (NULL != node->next) node = node->next;

	return node;
}

static void release(List **pself) {
	List *self = *pself;
	Allocator *a = self->a;

	ListNode *node = self->head;

	while (NULL != node) {
		ListNode *next = node->next;
		a->free(a, node);
		node = next;
	}

	a->free(a, self);
	*pself = NULL;
}

const List LIST_PROTOTYPE = {
	.prepend = prepend,
	.append = append,
	.remove = remove,
	.find = find,
	.tail = tail,
	.release = release
};

List* form_list(Allocator *a) {
	List *list = a->alloc(a, sizeof(List));

	*list = LIST_PROTOTYPE;
	list->a = a;
	list->head = NULL;
	list->cmp = &ptr_comparator;
	return list;
}