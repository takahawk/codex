#!/bin/sh

CC=${CC:=gcc}
LIBPATH="-L./build"
LIBS="-lcodex"
TESTS=tests/*.c
INCLUDE="-I./"
CFLAGS="-Wno-discarded-qualifiers -Wno-format-truncation -no-pie"

# for includes
ln -s . codex 

for test in $TESTS; do
	echo "Running test $test..."
	testbin="$test.bin"
	$CC $CFLAGS -no-pie -g -o "$testbin" $INCLUDE $test $LIBPATH $LIBS
	if ! ./$testbin > /dev/null; then
		echo "FAILURE"
		exit 1
	else
		echo "SUCCESS"
		rm $testbin
	fi
done

unlink codex