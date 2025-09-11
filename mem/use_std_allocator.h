#undef CDX_ALLOC
#undef CDX_CALLOC
#undef CDX_REALLOC
#undef CDX_FREE
#define CDX_ALLOC   malloc
#define CDX_CALLOC  calloc
#define CDX_REALLOC realloc
#define CDX_FREE    free
