#!/bin/bash
set -e

echo "======================================="
echo "Use Apple Silicon Homebrew + Qt"
echo "======================================="

if [ ! -x "/opt/homebrew/bin/brew" ]; then
    echo "ERROR: ARM Homebrew not found at /opt/homebrew/bin/brew"
    echo "Install ARM Homebrew first."
    exit 1
fi

eval "$(/opt/homebrew/bin/brew shellenv)"

echo
echo "Using brew:"
which brew

echo
echo "Qt prefix:"
brew --prefix qt

echo
echo "QtWidgets architecture:"
file "$(brew --prefix qt)/lib/QtWidgets.framework/Versions/A/QtWidgets"

echo
echo "Now run:"
echo "  ./scripts/clean.sh"
echo "  ./scripts/configure_qt.sh"
echo "  ./scripts/build.sh"