#!/bin/bash

if [ $# -ne 2 ]; then
	echo "arm64-direct-gdb.sh workspacefolder arm32filename"
	exit 1
fi

# 交叉编译程序成ARM32程序
aarch64-linux-gnu-gcc -g -static --include "$1/tests/std.h" -o "$1/tests/$2" "tests/$2.c" "$1/tests/std.c"

echo "Now run gdb in another window"
echo "Now gdb start"

# 通过qemu运行，同时开启gdb server功能
qemu-aarch64-static -g 1234 "$1/tests/$2"

echo
echo "Now gdb end"

exit 0