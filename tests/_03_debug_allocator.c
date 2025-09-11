#include "codex/testing/assert.h"
#include "codex/mem/dbg_allocator.h"
#include "codex/ds/array.h"

int main() {
  CdxDbgAllocator a = cdx_dbg_allocator_form();
  #define CDX_DBG_ALLOCATOR a
  #include "codex/mem/use_dbg_allocator.h"

	int *p1 = CDX_ALLOC(sizeof(*p1));
	int *p2 = CDX_ALLOC(sizeof(*p2));

	assert_uint16_equals(a.allocations.len, 2);

	Array arr = cdx_array_form(sizeof(int));
	assert_uint16_equals(a.allocations.len, 3);

	cdx_array_release(&arr);
	assert_uint16_equals(a.allocations.len, 2);

	CDX_FREE(p1);
	CDX_FREE(p2);
	assert_uint16_equals(a.allocations.len, 0);

  cdx_dbg_allocator_release(&a);

	return 0;
}
