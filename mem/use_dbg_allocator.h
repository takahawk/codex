#undef CDX_ALLOC
#undef CDX_CALLOC
#undef CDX_REALLOC
#undef CDX_FREE
#define CDX_ALLOC   cdx_dbg_allocator_alloc
#define CDX_CALLOC  cdx_dbg_allocator_calloc
#define CDX_REALLOC cdx_dbg_allocator_realloc
#define CDX_FREE    cdx_dbg_allocator_free
