#!/bin/bash

# set -xe
set -e

compiler="gcc"
build_dir="./dist"
src_dir="./src"
bin_name="program"

files="$(find $src_dir -name '*.c')"
# replace glfw with lraylib
mode=$(if [ "$1" = "debug" ]; then echo "-g3 -DDEBUG"; else echo ""; fi)
cflags="-Wall -Werror -Wextra -pedantic $mode"
libs="-lglfw -lGL -lm -lpthread -ldl -lrt -lX11"
includes="-I./include"

mkdir -p $build_dir

$compiler $files -o $build_dir/bin_name $cflags $libs $includes

if [ "$1" = "run" ]; then
    $build_dir/bin_name
fi
