#!/bin/bash
set -e

TOP_DIR="$(cd "$(dirname "$0")" && pwd)"
NSIS_DIR="$TOP_DIR/nsis"
DIST_DIR="$NSIS_DIR/Sylpheed"

echo "=========================================="
echo " 1. Building Sylpheed"
echo "=========================================="
cd "$TOP_DIR"
make -j$(nproc 2>/dev/null || echo 4)

echo "=========================================="
echo " 2. Preparing packaging directories"
echo "=========================================="
rm -rf "$DIST_DIR" "$NSIS_DIR/plugins"
mkdir -p "$DIST_DIR"
mkdir -p "$DIST_DIR/plugins"
mkdir -p "$DIST_DIR/doc/manual"
mkdir -p "$DIST_DIR/doc/faq"
mkdir -p "$DIST_DIR/share/icons"
mkdir -p "$DIST_DIR/share/locale"
mkdir -p "$DIST_DIR/lib/gdk-pixbuf-2.0/2.10.0/loaders"

mkdir -p "$NSIS_DIR/plugins/plugins"
mkdir -p "$NSIS_DIR/plugins/doc/plugins"

echo "=========================================="
echo " 3. Copying binaries and resources"
echo "=========================================="
cp src/.libs/sylpheed.exe "$DIST_DIR/"
cp src/.libs/syl-auth-helper.exe "$DIST_DIR/"
cp libsylph/.libs/libsylph-0-1.dll "$DIST_DIR/"
cp src/.libs/libsylpheed-plugin-0-1.dll "$DIST_DIR/"

# Plugin
if [ -f plugin/attachment_tool/.libs/attachment_tool.dll ]; then
    cp plugin/attachment_tool/.libs/attachment_tool.dll "$NSIS_DIR/plugins/plugins/"
    cp plugin/attachment_tool/.libs/attachment_tool.dll "$DIST_DIR/plugins/"
fi
if [ -f plugin/attachment_tool/README ]; then
    cp plugin/attachment_tool/README "$NSIS_DIR/plugins/doc/plugins/README.attachment_tool.txt"
fi

# Manuals and Docs
if [ -d manual ]; then
    cp -r manual/ja "$DIST_DIR/doc/manual/" 2>/dev/null || true
    cp -r manual/en "$DIST_DIR/doc/manual/" 2>/dev/null || true
fi
if [ -d faq ]; then
    cp -r faq/* "$DIST_DIR/doc/faq/" 2>/dev/null || true
fi

cp COPYING "$DIST_DIR/COPYING.txt" 2>/dev/null || true
cp nsis/COPYING.ja.txt "$DIST_DIR/COPYING.ja.txt" 2>/dev/null || true
cp README "$DIST_DIR/README.txt" 2>/dev/null || true
cp README.ja "$DIST_DIR/README-win32-ja.txt" 2>/dev/null || true
cp README.es "$DIST_DIR/README-win32-es.txt" 2>/dev/null || true
cp README "$DIST_DIR/README-win32.txt" 2>/dev/null || true

# Icons
if [ -d src/icons ]; then
    cp src/icons/*.png "$DIST_DIR/share/icons/" 2>/dev/null || true
fi

# Locales
for gmo in po/*.gmo; do
    if [ -f "$gmo" ]; then
        lang=$(basename "$gmo" .gmo)
        mkdir -p "$DIST_DIR/share/locale/$lang/LC_MESSAGES"
        cp "$gmo" "$DIST_DIR/share/locale/$lang/LC_MESSAGES/sylpheed.mo"
    fi
done

# GDK-Pixbuf loaders
if [ -d /mingw64/lib/gdk-pixbuf-2.0/2.10.0/loaders ]; then
    cp /mingw64/lib/gdk-pixbuf-2.0/2.10.0/loaders/*.dll "$DIST_DIR/lib/gdk-pixbuf-2.0/2.10.0/loaders/" 2>/dev/null || true
fi
if [ -f /mingw64/lib/gdk-pixbuf-2.0/2.10.0/loaders.cache ]; then
    cp /mingw64/lib/gdk-pixbuf-2.0/2.10.0/loaders.cache "$DIST_DIR/lib/gdk-pixbuf-2.0/2.10.0/"
fi

echo "=========================================="
echo " 4. Collecting DLL dependencies"
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
        if [ -z "${copied_dlls[$dll]}" ]; then
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

echo "=========================================="
echo " 5. Building updater components"
echo "=========================================="
cd "$NSIS_DIR"
makensis plugin-updater.nsi
mv -f plugin-updater.exe "$DIST_DIR/"
makensis update-manager.nsi
mv -f update-manager.exe "$DIST_DIR/"

echo "=========================================="
echo " 6. Building Sylpheed Installer"
echo "=========================================="
makensis sylpheed.nsi

echo "=========================================="
echo " Build Completed Successfully!"
echo "=========================================="
ls -lh "$NSIS_DIR"/*_setup.exe
