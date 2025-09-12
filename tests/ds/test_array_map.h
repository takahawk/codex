#include "codex/testing/assert.h"
#include "codex/testing/cdx_test.h"

#include <string.h>

#include "codex/ds/array_map.h"

static bool test_add_get() {
  ArrayMap map = cdx_array_map_form(sizeof(char*));
  cdx_array_map_add(&map, "zero", "one");
  char *zero = cdx_array_map_get(map, "zero");
  // TODO: rework asserts
  return strcmp(zero, "one") == 0;
}

CdxTestSuite ds_array_map = {
  .title = "ArrayMap",
  .filename = __FILE__,
  .tests = (CdxTest[]) {
    { "simple add and get", test_add_get },
    { NULL, NULL }
  }
};
