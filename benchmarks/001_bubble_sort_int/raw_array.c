#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
  int n = atoi(argv[1]);

  int *a = malloc(sizeof(int) * n);

  for (int i = 0; i < n; i++) {
    a[i] = n - i;
  }

  double start, end, cpu_time_used;
  start = clock();

  int i, t, j = n, s = 1;
  while (s) {
    s = 0;
    for (i = 1; i < j; i++) {
      if (a[i] < a[i - 1]) {
        t = a[i];
        a[i] = a[i - 1];
        a[i - 1] = t;
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
