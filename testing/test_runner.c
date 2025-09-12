#include <stdbool.h>

#include "codex/testing/cdx_test.h"

#ifndef SUITE_HEADER
  #error "Define SUITE_HEADER=path/to/suite.h"
#endif

#ifndef SUITE_SYMBOL
  #error "Define SUITE_SYMBOL=cdx_suite_variable_name"
#endif

#define STRINGIFY(x) #x
#define EXPAND_AND_STRINGIFY(X) STRINGIFY(X)

#include EXPAND_AND_STRINGIFY(SUITE_HEADER)

int main(void) {
  return cdx_suite_run_tests(SUITE_SYMBOL) == true;
}

