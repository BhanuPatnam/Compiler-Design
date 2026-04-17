#!/bin/bash
# Script to build a Debian package for the alg compiler

set -e

# Configuration
PACKAGE_NAME="alg-compiler"
VERSION="1.0-1"
MAINTAINER="Bhanu Prakash <patnambhanuprakash@gmail.com>"
DESCRIPTION="A simple compiler for the ALG language."
EXTENDED_DESCRIPTION=" The ALG compiler translates .alg files to C and then compiles them using gcc. It follows a 7-phase architecture: Lexer, Parser, AST, Semantic, ICG, CodeGen, and Linking."

# Architecture detection
ARCH=$(dpkg --print-architecture 2>/dev/null || echo "amd64")

# Directory setup
BUILD_DIR="pkg_debian"
DEB_FILE="${PACKAGE_NAME}_${VERSION}_${ARCH}.deb"

echo "--- Building ${PACKAGE_NAME} (${VERSION}) for ${ARCH} ---"

# 1. Build the compiler
# Note: This step might fail on non-Linux systems due to dependencies or architecture
if [ -d "compiler" ]; then
    echo "--- Compiling source ---"
    cd compiler
    make clean || true
    make all || echo "WARNING: Compilation failed. Using existing binary if available."
    cd ..
fi

# 2. Prepare package structure
rm -rf ${BUILD_DIR}
mkdir -p ${BUILD_DIR}/DEBIAN
mkdir -p ${BUILD_DIR}/usr/bin

# 3. Copy binary
if [ -f "alg" ]; then
    cp alg ${BUILD_DIR}/usr/bin/alg
    chmod 755 ${BUILD_DIR}/usr/bin/alg
else
    echo "ERROR: 'alg' binary not found. Please build the project first."
    exit 1
fi

# 4. Create control file
cat <<EOF > ${BUILD_DIR}/DEBIAN/control
Package: ${PACKAGE_NAME}
Version: ${VERSION}
Section: devel
Priority: optional
Architecture: ${ARCH}
Maintainer: ${MAINTAINER}
Depends: gcc, libc6
Description: ${DESCRIPTION}
${EXTENDED_DESCRIPTION}
EOF

# 5. Create postinst script
cat <<EOF > ${BUILD_DIR}/DEBIAN/postinst
#!/bin/bash
set -e
if [ "\$1" = "configure" ]; then
    chmod 755 /usr/bin/alg
    echo "alg-compiler installed successfully in /usr/bin/alg"
fi
exit 0
EOF
chmod 755 ${BUILD_DIR}/DEBIAN/postinst

# 6. Build the .deb package
if command -v dpkg-deb &> /dev/null; then
    echo "--- Creating Debian package ---"
    dpkg-deb --build ${BUILD_DIR} ${DEB_FILE}
    echo "SUCCESS: Package created as ${DEB_FILE}"
else
    echo "--- Package structure ready ---"
    echo "WARNING: 'dpkg-deb' not found. The package structure is ready in ${BUILD_DIR}/."
    echo "To create the .deb file on a Debian/Ubuntu system, run:"
    echo "  dpkg-deb --build ${BUILD_DIR} ${DEB_FILE}"
fi
