#!/bin/sh

BUILD_TYPE="Release"

if [ "$#" == 0 ]; then
    BUILD_TYPE="Release"
else
    BUILD_TYPE="$1"
fi

mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" ../
make
cd ..
cp build/compile_commands.json .

