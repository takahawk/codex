#ifndef CDX_TEST_BASE_H_
#define CDX_TEST_BASE_H_
#include <stdio.h>

typedef bool (*CdxTestFunc)(void);

typedef struct {
  char *name;
  CdxTestFunc test_func;
} CdxTest;

typedef struct {
  char *title;
  char *filename;
  CdxTest *tests;
} CdxTestSuite;

static void cdx_test_suite_run(CdxTestSuite suite) {
  printf("RUNNING TEST SUITE\n");
  printf("\"%s\" (in %s)\n", suite.title, suite.filename);
  printf("===========================================================\n");
  int total = 0;
  int succeeded = 0;
  for (int i = 0; suite.tests[i].test_func != NULL; i++) {
    CdxTest test = suite.tests[i];
    printf("\t%s... ", test.name);
    bool success = test.test_func();
    // TODO: colors
    printf("%s\n", success ? "SUCCESS" : "FAILURE");
    total++;
    if (success)
      succeeded++;
  }

  printf("\tSUMMARY: %d out of %d succeeded\n\n", succeeded, total);

  // TODO: catch sigsegv?
}

#endif
