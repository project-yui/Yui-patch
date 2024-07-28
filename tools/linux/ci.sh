#!/bin/bash
set -ex
root_dir=$(cd `dirname $0`/../.. && pwd -P)

arch=$1
tag=$2

cmake --no-warn-unused-cli -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -S"$root_dir" -B"$root_dir/build" -G Ninja
cmake --build "$root_dir/build" --config Release --target all --

mkdir -p "$root_dir/tmp/build"

for file in "$root_dir/build/"*.so
do
    name=$(basename $file)
    mv $file "$root_dir/tmp/build/yukihana-${name%.*}-linux-$arch-$tag.so"
done