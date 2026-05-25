#!/bin/bash
set -e

echo "======================================="
echo "SCPI C++ Demo - Configure CMake + Qt"
echo "======================================="

QT_PREFIX="/opt/homebrew/opt/qt"

if [ ! -d "$QT_PREFIX" ]; then
    echo "ERROR: ARM Qt not found at $QT_PREFIX"
    echo "Run:"
    echo "  /opt/homebrew/bin/brew install qt"
    exit 1
fi

echo
echo "Using Qt from:"
echo "$QT_PREFIX"

cmake -S . -B build -DCMAKE_PREFIX_PATH="$QT_PREFIX"

echo
echo "Configure complete."
echo "Next run:"
echo "  cmake --build build"