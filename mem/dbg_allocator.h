#ifndef CDX_DBG_ALLOCATOR_H_
#define CDX_DBG_ALLOCATOR_H_
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "codex/ds/array.h"

typedef struct {
	void *ptr;
	size_t size;

	void *stack[16];
	int stack_size;
} _CdxDbgAllocationEntry;

typedef struct {
  Array/*_CdxDbgAllocationEntry*/ allocations;
  size_t total_allocated;
  size_t total_freed;
} CdxDbgAllocator;

static inline CdxDbgAllocator cdx_dbg_allocator_form() {
  return (CdxDbgAllocator) {
    .allocations     = cdx_array_form(sizeof(_CdxDbgAllocationEntry)),
    .total_allocated = 0,
    .total_freed     = 0,
  };
}

static inline void cdx_dbg_allocator_release(CdxDbgAllocator *a) {
  cdx_array_release(&a->allocations);
}

static void _cdx_dbg_allocator_add_entry(CdxDbgAllocator *a, void *ptr, size_t size) {
	void *stack[16];
	int stack_size = backtrace(stack, 16);
	_CdxDbgAllocationEntry entry = {
		.ptr = ptr,
		.size = size,
		.stack_size = stack_size
	};
	memcpy(entry.stack, stack, sizeof(void*) * stack_size);

	a->total_allocated += size;

	cdx_array_add(&(a->allocations), &entry);
}

static void inline _cdx_dbg_allocator_remove_entry(CdxDbgAllocator *a, void *ptr) {
	Array/*_CdxDbgAllocationEntry*/ *al = &(a->allocations);

	for (size_t i = 0; i < al->len; ++i) {
		_CdxDbgAllocationEntry *entry = cdx_array_get(*al, i);
		if (entry->ptr == ptr) {
			a->total_freed += entry->size;
			cdx_array_fremove(al, i);
			return;
		}
	}

	fprintf(stderr, "attempt to free non-allocated pointer: %p", ptr);
}

static inline void* cdx_dbg_allocator_alloc(CdxDbgAllocator *a, size_t size) {
	void* p = malloc(size);
	_cdx_dbg_allocator_add_entry(a, p, size);

	return p;
}

static void* cdx_dbg_allocator_realloc(CdxDbgAllocator *a, void *p, size_t size) {
	_cdx_dbg_allocator_remove_entry(a, p);
	p = realloc(p, size);
	_cdx_dbg_allocator_add_entry(a, p, size);

	return p;
}

static void
cdx_dbg_allocator_free(CdxDbgAllocator *a, void *p) {
	_cdx_dbg_allocator_remove_entry(a, p);
	free(p);
}

static void cdx_dbg_allocator_print_allocations(CdxDbgAllocator a) {
  Array *al = &a.allocations;
	char exe_path[1024];
	readlink("/proc/self/exe", exe_path, sizeof(exe_path));
	fprintf(stderr, "LIST OF ACTIVE ALLOCATIONS:\n");
  fprintf(stderr, "================================================\n\n");
	for (size_t i = 0; i < al->len; ++i) {
		_CdxDbgAllocationEntry *entry = cdx_array_get(*al, i);
		// start with 2, because first two lines are happening inside allocator
		for (size_t j = 2; j < entry->stack_size; ++j) {
			char cmd[2048];
			snprintf(cmd, sizeof(cmd), "addr2line -e %s %p", exe_path, entry->stack[j]);
			FILE *fp = popen(cmd, "r");

			char line[512];
			while (fgets(line, sizeof(line), fp)) {
				if (strstr(line, "??:0") || strstr(line, "??:?"))
					continue;

				fprintf(stderr, "  → %s", line);
			}
		}
		fprintf(stderr, "------------------------------------------------\n\n");
	}
}

#endif
