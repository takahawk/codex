#ifndef CODEX_RELEASE_CB_
#define CODEX_RELEASE_CB_
#include "codex/mem/allocator.h"

typedef struct ReleaseCb ReleaseCb;

struct ReleaseCb {
  Allocator *a;
  void (*cb) (Allocator *a, void **ptr);
  // ctx?
};

struct _ReleaseCbStatic {
  const ReleaseCb nop;

  const ReleaseCb (*form)         (Allocator *a, void (*cb) (Allocator *a, void **ptr));
  const ReleaseCb (*form_free_cb) (Allocator *a);
};

static ReleaseCb _release_cb_form(Allocator *a, void (*cb) (Allocator *a, void **ptr))
{
  return (ReleaseCb) {
    .a  = a,
    .cb = cb,
  };
}

static void _release_cb_nop(Allocator *a, void **ptr)
{
  // do nothing
}

static void _release_cb_free(Allocator *a, void **ptr)
{
  void *val = *ptr;
  a->free(a, val);
  *ptr = NULL;
}

static ReleaseCb _release_cb_form_free_cb(Allocator *a)
{
  return (ReleaseCb) {
    .a = a,
    .cb = _release_cb_free
  };
}

static const struct _ReleaseCbStatic RELEASE_CB = {
  .nop = {
    .a = NULL,
    .cb = _release_cb_nop,
  },

  .form = _release_cb_form,
  .form_free_cb = _release_cb_form_free_cb,
};

#endif // !CODEX_RELEASE_CB_
