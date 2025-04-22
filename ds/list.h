#ifndef CODEX_LIST_H_
#define CODEX_LIST_H_
#include "codex/mem/allocator.h"
#include "codex/ds/comparator.h"

/*
 * Just a list. A linked one
 */
typedef struct ListNode ListNode;
struct ListNode {
	void     *value;
	ListNode *next;
};

typedef struct List List;
struct List {
	Allocator  *a;
	Comparator *cmp;
	ListNode   *head;

	void      (*prepend) (List *self, void *value);
	void      (*append)  (List *self, void *value);

	// uses comparator which is ptr_comparator by default
	void      (*remove)  (List *self, void *value);
	ListNode* (*find)    (List *self, void *value);

	ListNode* (*tail)    (List *self);

	void      (*release) (List **pself);
};

extern const List LIST_PROTOTYPE;

List* form_list(Allocator *a);

#endif
