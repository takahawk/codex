#!/bin/sh

CC=${CC:=gcc}
SRCS="ds/array.c parsers/dotenv.c allocators/std_allocator.c"
OBJS=""
LIBNAME=build/libcodex.a
INCLUDE="-I./"

set -x

mkdir -p build
mkdir -p build/ds
mkdir -p build/testing
mkdir -p build/parsers
mkdir -p build/allocators

for src in $SRCS; do
	obj="build/${src%.c}.o"
	gcc -g -c "$src" -o "$obj" $INCLUDE
	OBJS="$OBJS $obj"
done

ar rcs "$LIBNAME" $OBJS
