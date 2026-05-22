#!/bin/bash

set -e

echo "======================================="
echo "SCPI C++ Qt Demo Launcher"
echo "======================================="

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

cd "$PROJECT_ROOT"

echo
echo "[1/4] Cleaning build..."
./scripts/clean.sh

echo
echo "[2/4] Configuring CMake + Qt..."
./scripts/configure_qt.sh

echo
echo "[3/4] Building project..."
./scripts/build.sh

echo
echo "[4/4] Launching Qt GUI..."

./build/scpi_cpp_qt_demo

echo
echo "Qt application exited."