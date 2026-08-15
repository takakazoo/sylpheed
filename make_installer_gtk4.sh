#!/bin/bash
# ==============================================================================
# Sylpheed-GTK4 Windows NSIS Installer Generator
# ==============================================================================
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

DIST_DIR="$SCRIPT_DIR/nsis/Sylpheed-GTK4"
NSIS_DIR="$SCRIPT_DIR/nsis"

echo "=========================================="
echo " 1. Building Sylpheed-GTK4"
echo "=========================================="
make -C src-gtk4 -j$(nproc)

echo "=========================================="
echo " 2. Preparing packaging directories"
echo "=========================================="
rm -rf "$DIST_DIR"
mkdir -p "$DIST_DIR"
mkdir -p "$DIST_DIR/etc/ssl/certs"
mkdir -p "$DIST_DIR/share/icons"
mkdir -p "$DIST_DIR/share/locale"
mkdir -p "$DIST_DIR/share/glib-2.0/schemas"

echo "=========================================="
echo " 3. Copying binaries and resources"
echo "=========================================="
cp src-gtk4/.libs/sylpheed-gtk4.exe "$DIST_DIR/sylpheed.exe"
cp libsylph/.libs/libsylph-0-1.dll "$DIST_DIR/"
cp README "$DIST_DIR/README.txt" 2>/dev/null || true
cp README.ja "$DIST_DIR/README-win32-ja.txt" 2>/dev/null || true
cp README.es "$DIST_DIR/README-win32-es.txt" 2>/dev/null || true
cp README "$DIST_DIR/README-win32.txt" 2>/dev/null || true
cp COPYING "$DIST_DIR/COPYING.txt" 2>/dev/null || true
cp nsis/COPYING.ja.txt "$DIST_DIR/COPYING.ja.txt" 2>/dev/null || true
cp oauth2.ini.sample "$DIST_DIR/oauth2.ini.sample" 2>/dev/null || true
cp OAUTH2.md "$DIST_DIR/OAUTH2.txt" 2>/dev/null || true

# SSL Root CA Certificates
if [ -f /mingw64/etc/ssl/certs/ca-bundle.crt ]; then
    cp /mingw64/etc/ssl/certs/ca-bundle.crt "$DIST_DIR/etc/ssl/certs/"
fi

# GSettings Schemas (Required for GTK4)
if [ -f /mingw64/share/glib-2.0/schemas/gschemas.compiled ]; then
    cp /mingw64/share/glib-2.0/schemas/gschemas.compiled "$DIST_DIR/share/glib-2.0/schemas/"
fi

echo "=========================================="
echo " 4. Collecting GTK4 DLL dependencies"
echo "=========================================="
declare -A copied_dlls

queue=()
for f in $(find "$DIST_DIR" -type f \( -name '*.exe' -o -name '*.dll' \)); do
    queue+=("$f")
done

while [ ${#queue[@]} -gt 0 ]; do
    current="${queue[0]}"
    queue=("${queue[@]:1}")
    
    for dll in $(objdump -p "$current" 2>/dev/null | grep -i 'DLL Name:' | awk '{print $3}'); do
        if [ -z "${copied_dlls[$dll]:-}" ]; then
            copied_dlls[$dll]=1
            if [ -f "/mingw64/bin/$dll" ]; then
                if [ ! -f "$DIST_DIR/$dll" ]; then
                    cp "/mingw64/bin/$dll" "$DIST_DIR/"
                    queue+=("$DIST_DIR/$dll")
                fi
            fi
        fi
    done
done

echo "Collected ${#copied_dlls[@]} DLL dependencies."

echo "=========================================="
echo " 5. Building GTK4 Sylpheed Installer"
echo "=========================================="
cd "$NSIS_DIR"

rm -rf Sylpheed-GTK2-bak
if [ -d "Sylpheed" ]; then
    mv Sylpheed Sylpheed-GTK2-bak
fi
cp -r Sylpheed-GTK4 Sylpheed

makensis -V2 -DINST_NAME="Sylpheed (GTK4 Preview)" -DINST_VERSION="4.0.0-alpha" -DINST_FILENAME="Sylpheed-GTK4" sylpheed.nsi

rm -rf Sylpheed
if [ -d "Sylpheed-GTK2-bak" ]; then
    mv Sylpheed-GTK2-bak Sylpheed
fi

echo "=========================================="
echo " Build Completed Successfully!"
echo "=========================================="
ls -lh "$NSIS_DIR/Sylpheed-GTK4-4.0.0-alpha_setup.exe"
