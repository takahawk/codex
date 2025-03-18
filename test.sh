#!/bin/sh

CC=${CC:=gcc}
LIBPATH="-L./build"
LIBS="-lcodex"
TESTS=tests/*.c
INCLUDE="-I./"

set -x
for test in $TESTS; do
	echo "Running test $test..."
	testbin="$test.bin"
	$CC -g -o "$testbin" $INCLUDE $test $LIBPATH $LIBS
	if ! ./$testbin > /dev/null; then
		echo "FAILURE"
		exit 1
	else
		echo "SUCCESS"
		rm $testbin
	fi
done
