import sys
import time

n = int(sys.argv[1])

a = []

for i in range(n):
    a.append(n - i)

start = time.perf_counter()
changed = True
while changed:
    changed = False
    for i in range(len(a) - 1):
        if a[i] > a[i+1]:
            a[i], a[i+1] = a[i+1], a[i]
            changed = True

end = time.perf_counter()
cpu_time = end - start

print(f"{end - start:.6f}")
