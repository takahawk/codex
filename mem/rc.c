#include "codex/mem/rc.h"

#include <stdlib.h>

#include "codex/mem/allocator.h"

static Rc* form_with_alloc(Allocator *a, void *data, ReleaseCb cb)
{
  Rc *rc = malloc(sizeof(Rc));
  rc->a = a;
  rc->data = data;
  rc->counter = 1;
  rc->cb = cb;

  return rc;
}

static Rc* form(void *data, ReleaseCb cb)
{
  return form_with_alloc(&STD_ALLOCATOR, data, cb);
}

static Rc* retain(Rc *rc)
{
  rc->counter++;
  return rc;
}

static void release(Rc **prc)
{
  Rc *rc = *prc;
  rc->counter--;
  if (!rc->counter) {
    rc->cb.cb(rc->a, &rc->data);
    free(rc);
  }

  *prc = NULL;
}

const struct _RcStatic RC = {
  .form = form,
  .form_with_alloc = form_with_alloc,
  .retain = retain,
  .release = release,
};
