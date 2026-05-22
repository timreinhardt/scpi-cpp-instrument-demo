#!/bin/bash
set -e

echo "======================================="
echo "SCPI C++ Demo - Configure CMake + Qt"
echo "======================================="

QT_PREFIX="$(brew --prefix qt)"

echo
echo "Using Qt from:"
echo "$QT_PREFIX"

echo
echo "Generating build system into ./build ..."
cmake -S . -B build -DCMAKE_PREFIX_PATH="$QT_PREFIX"

echo
echo "Configure complete."
echo "Next run:"
echo "  cmake --build build"
echo
