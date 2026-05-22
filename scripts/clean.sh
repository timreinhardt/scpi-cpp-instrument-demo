#!/bin/bash

set -e

echo "======================================="
echo "SCPI C++ Demo - Bruteforce Clean"
echo "======================================="

if [ -d "build" ]; then
    echo
    echo "Removing build directory..."
    rm -rf build
    echo "Build directory removed."
else
    echo
    echo "No build directory found."
fi

echo
echo "Clean complete."