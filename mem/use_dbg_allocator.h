#ifndef CDX_DBG_ALLOCATOR 
  #error "You should set CDX_DBG_ALLOCATOR before using it"
#else
  CdxDbgAllocator *_cdx_dbg_allocator = &CDX_DBG_ALLOCATOR;
#endif

#undef CDX_ALLOC
#undef CDX_REALLOC
#undef CDX_FREE
#define CDX_ALLOC(size)      cdx_dbg_allocator_alloc(_cdx_dbg_allocator, size)
#define CDX_REALLOC(p, size) cdx_dbg_allocator_realloc(_cdx_dbg_allocator, p, size)
#define CDX_FREE(p)          cdx_dbg_allocator_free(_cdx_dbg_allocator, p)
