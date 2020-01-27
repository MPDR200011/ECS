#!/bin/sh

mkdir build
cmake -B build
cmake --build build/ --target $1
cp build/$1 -t .