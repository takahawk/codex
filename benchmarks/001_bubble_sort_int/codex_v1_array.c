#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "codex/v1/ds/array.h"

int main(int argc, char **argv)
{
  int n = atoi(argv[1]);

  Array* a = ARRAY.form(sizeof(int));
  double start, end, cpu_time_used;

  for (int i = 0; i < n; i++) {
    int num = n - i;
    a->add(a, &num);
  }

  start = clock();
  int i, t, j = n, s = 1;
  while (s) {
    s = 0;
    for (i = 1; i < j; i++) {
      if (*(int *) a->get(a, i) < *(int *) a->get(a, i - 1)) {
        t = *(int *)(a->get(a, i));
        a->set(a, i, a->get(a, i - 1));
        a->set(a, i - 1, &t);
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
