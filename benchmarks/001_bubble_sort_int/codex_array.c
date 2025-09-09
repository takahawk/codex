#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "codex/ds/array.h"

int main(int argc, char **argv)
{
  int n = atoi(argv[1]);

  Array a = cdx_array_form(sizeof(int));
  double start, end, cpu_time_used;

  for (int i = 0; i < n; i++) {
    int num = n - i;
    cdx_array_add(&a, &num);
  }

  start = clock();
  int i, t, j = n, s = 1;
  while (s) {
    s = 0;
    for (i = 1; i < j; i++) {
      int rhs = *(int *) cdx_array_get(a, i);
      int lhs = *(int *) cdx_array_get(a, i - 1);
      if (rhs < lhs) {
        cdx_array_set(a, i, &lhs);
        cdx_array_set(a, i - 1, &rhs);
        s = 1;
      }
    }
    j--;
  }

  end = clock();

  cpu_time_used = ((double) end - start) / CLOCKS_PER_SEC;

  printf("%.6f\n", cpu_time_used);

  return 0;
}
