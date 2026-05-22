#!/bin/bash
set -e

echo "======================================="
echo "SCPI C++ Demo - Build"
echo "======================================="

if [ ! -d "build" ]; then
    echo "No build directory found."
    echo "Run this first:"
    echo "  ./scripts/configure_qt.sh"
    exit 1
fi

cmake --build build

echo
echo "Built executables:"
echo "  build/scpi_cpp_demo"
echo "  build/scpi_cpp_qt_demo"
