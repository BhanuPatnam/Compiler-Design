#!/bin/bash
set -e

make

mkdir -p package/usr/bin
cp algocompiler package/usr/bin/algocompiler
chmod 755 package/usr/bin/algocompiler

chmod 755 package/DEBIAN/postinst

dpkg-deb --build package algocompiler_1.0_amd64.deb

echo "Done! Share the algocompiler_1.0_amd64.deb file"

