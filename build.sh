#!/bin/sh

CC=${CC:=gcc}
SRCS="ds/array.c parsers/dotenv.c"
OBJS=""
LIBNAME=libcodex.a

set -x

for src in $SRCS; do
	obj="${src%.c}.o"
	gcc -c "$src" -o "$obj"
	OBJS="$OBJS $obj"
done

ar rcs "$LIBNAME" $OBJS
