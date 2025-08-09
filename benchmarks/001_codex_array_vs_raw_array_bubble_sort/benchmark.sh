#!/bin/sh

CC=${CC:=gcc}
INCLUDE="-I./"
CFLAGS="-Wno-discarded-qualifiers"

ln -s ../../ codex 

echo "CODEX ARRAY VS RAW ARRAY - BUBBLE SORT"
echo "======================================"
N=10000
echo "N=$N"

$CC raw_array.c -o bnch.bin 
cpu_time=$(./bnch.bin $N)
echo "Raw array (seconds): $cpu_time"

$CC -O1 raw_array.c -o bnch.bin 
cpu_time=$(./bnch.bin $N)
echo "Raw array with -O1 (seconds): $cpu_time"

$CC -O2 raw_array.c -o bnch.bin 
cpu_time=$(./bnch.bin $N)
echo "Raw array with -O2 (seconds): $cpu_time"

$CC -O3 raw_array.c -o bnch.bin 
cpu_time=$(./bnch.bin $N)
echo "Raw array with -O3 (seconds): $cpu_time"

$CC codex_array.c codex/ds/array.c codex/mem/std_allocator.c -o bnch.bin $INCLUDE $CFLAGS
cpu_time=$(./bnch.bin $N)
echo "Codex array (seconds): $cpu_time"

$CC -O1 codex_array.c codex/ds/array.c codex/mem/std_allocator.c -o bnch.bin $INCLUDE $CFLAGS
cpu_time=$(./bnch.bin $N)
echo "Codex array with -O1 (seconds): $cpu_time"

$CC -O2 codex_array.c codex/ds/array.c codex/mem/std_allocator.c -o bnch.bin $INCLUDE $CFLAGS
cpu_time=$(./bnch.bin $N)
echo "Codex array with -O2 (seconds): $cpu_time"

$CC -O3 codex_array.c codex/ds/array.c codex/mem/std_allocator.c -o bnch.bin $INCLUDE $CFLAGS
cpu_time=$(./bnch.bin $N)
echo "Codex array with -O3 (seconds): $cpu_time"

cpu_time=$(python3 -O python_list.py $N)
echo "Python list (seconds): $cpu_time"

cpu_time=$(pypy3 python_list.py $N)
echo "Python list (PyPy) (seconds): $cpu_time"

rm bnch.bin
unlink codex
