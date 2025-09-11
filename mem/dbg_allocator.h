#ifndef CDX_DBG_ALLOCATOR
#define CDX_DBG_ALLOCATOR
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

} DebugAllocationEntry;

Array/*DebugAllocationEntry*/ _cdx_dbg_alloc_allocations;

size_t _cdx_dbg_alloc_total_allocated = 0;
size_t _cdx_dbg_alloc_total_freed = 0;

static void cdx_dbg_allocator_init() {
  _cdx_dbg_alloc_allocations = cdx_array_form(sizeof(DebugAllocationEntry));
}

static void cdx_dbg_allocator_clean() {
  cdx_array_release(&_cdx_dbg_alloc_allocations);
}

static void _cdx_dbg_allocator_add_entry(void *ptr, size_t size) {
	void *stack[16];
	int stack_size = backtrace(stack, 16);
	DebugAllocationEntry entry = {
		.ptr = ptr,
		.size = size,
		.stack_size = stack_size
	};
	memcpy(entry.stack, stack, sizeof(void*) * stack_size);

	_cdx_dbg_alloc_total_allocated += size;

	cdx_array_add(&_cdx_dbg_alloc_allocations, &entry);
}

static void inline _cdx_dbg_allocator_remove_entry(void *ptr) {
	Array/*DebugAllocationEntry*/ *al = &_cdx_dbg_alloc_allocations;

	for (size_t i = 0; i < al->len; ++i) {
		DebugAllocationEntry *entry = cdx_array_get(*al, i);
		if (entry->ptr == ptr) {
			_cdx_dbg_alloc_total_freed += entry->size;
			cdx_array_fremove(al, i);
			return;
		}
	}

	fprintf(stderr, "attempt to free non-allocated pointer: %p", ptr);
}

static inline void* cdx_dbg_allocator_alloc(size_t size) {
	void* p = malloc(size);
	_cdx_dbg_allocator_add_entry(p, size);

	return p;
}

static void* cdx_dbg_allocator_realloc(void *p, size_t size) {
	_cdx_dbg_allocator_remove_entry(p);
	p = realloc(p, size);
	_cdx_dbg_allocator_add_entry(p, size);

	return p;
}

static void
cdx_dbg_allocator_free(void *p) {
	_cdx_dbg_allocator_remove_entry(p);
	free(p);
}

static void cdx_dbg_allocator_print_allocations() {
  Array *al = &_cdx_dbg_alloc_allocations;
	char exe_path[1024];
	readlink("/proc/self/exe", exe_path, sizeof(exe_path));
	fprintf(stderr, "LIST OF ACTIVE ALLOCATIONS:\n");
  fprintf(stderr, "================================================\n\n");
	for (size_t i = 0; i < al->len; ++i) {
		DebugAllocationEntry *entry = cdx_array_get(*al, i);
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
