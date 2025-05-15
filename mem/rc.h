#ifndef CODEX_RC_
#define CODEX_RC_
#include "codex/mem/allocator.h"
#include "codex/mem/release_cb.h"

typedef struct Rc Rc;

struct Rc {
  Allocator *a;
  int counter;
  ReleaseCb cb;
  void *data;
};

struct _RcStatic {
  Rc* (*form)            (void *data, ReleaseCb cb);
  Rc* (*form_with_alloc) (Allocator *a, void *data, ReleaseCb cb);

  // increment counter by one and get another 
  Rc* (*retain)  (Rc *rc);

  // decrement counter by one and release if 0
  void (*release) (Rc **prc);
};

extern const struct _RcStatic RC;

#endif
