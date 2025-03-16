#!/bin/sh

CC=${CC:=gcc}
SRCS=$(find . -type f -name '*.c')
TESTS=tests/*.c

for test in $TESTS; do
	echo "Running test $test..."
	testbin="$test.bin"
	$CC -g -o "$testbin" $SRCS $test
	if ! ./$testbin > /dev/null; then
		echo "FAILURE"
		exit 1
	else
		echo "SUCCESS"
		rm $testbin
	fi
done
