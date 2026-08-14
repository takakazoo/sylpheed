#!/bin/bash
set -e

# Sylpheed Windows build and packaging helper for MSYS2 / MinGW-w64

TOP_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$TOP_DIR"

echo "=== 1. Generating build configuration ==="
if [ ! -f Makefile ]; then
    ./autogen.sh --enable-oniguruma --enable-threads --enable-ssl --enable-gpgme
fi

echo "=== 2. Building Sylpheed and Plugins ==="
make -j$(nproc 2>/dev/null || echo 4)

echo "=== 3. Creating Windows Installer ==="
./make_installer.sh

echo "=== Windows build and packaging completed successfully! ==="
