#include "codex/testing/assert.h"
#include "codex/mem/dbg_allocator.h"
#include "codex/ds/array.h"

int main() {
  cdx_dbg_allocator_init();
  #include "codex/mem/use_dbg_allocator.h"

	int *p1 = CDX_ALLOC(sizeof(*p1));
	int *p2 = CDX_ALLOC(sizeof(*p2));

	assert_uint16_equals(_cdx_dbg_alloc_allocations.len, 2);

	Array arr = cdx_array_form(sizeof(int));
	assert_uint16_equals(_cdx_dbg_alloc_allocations.len, 3);

	cdx_array_release(&arr);
	assert_uint16_equals(_cdx_dbg_alloc_allocations.len, 2);

	CDX_FREE(p1);
	CDX_FREE(p2);
	assert_uint16_equals(_cdx_dbg_alloc_allocations.len, 0);

	return 0;
}
