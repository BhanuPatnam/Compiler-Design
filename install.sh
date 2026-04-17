#!/bin/bash
# Universal AlgoCompiler One-Command Installer
# Works on macOS (Homebrew) and Debian-based Linux (apt)

set -e

# Configuration
PACKAGE_NAME="alg-compiler"
VERSION="1.0-1"
BINARY_NAME="alg"

echo "--- AlgoCompiler One-Command Installer ---"

# 1. Detect OS and install dependencies
if [[ "$OSTYPE" == "darwin"* ]]; then
    echo "Detected macOS. Checking for Homebrew..."
    if ! command -v brew &> /dev/null; then
        echo "Error: Homebrew is not installed. Please install it first from https://brew.sh/"
        exit 1
    fi
    echo "Installing dependencies (flex, bison, gcc)..."
    brew install flex bison gcc
elif [[ -f /etc/debian_version ]]; then
    echo "Detected Debian-based Linux. Updating and installing dependencies..."
    sudo apt update
    sudo apt install -y build-essential flex bison gcc
else
    echo "WARNING: Unsupported OS detected. Please ensure flex, bison, and gcc are installed manually."
fi

# 2. Build the project
echo "--- Building AlgoCompiler ---"
cd compiler
make clean
make all
cd ..

# 3. Installation
if [[ -f /etc/debian_version ]]; then
    echo "--- Creating and Installing Debian Package ---"
    ./scripts/build_deb.sh
    
    # Architecture detection for the filename
    ARCH=$(dpkg --print-architecture 2>/dev/null || echo "amd64")
    DEB_FILE="${PACKAGE_NAME}_${VERSION}_${ARCH}.deb"
    
    if [ -f "$DEB_FILE" ]; then
        sudo apt install -y ./"$DEB_FILE"
    else
        echo "Debian package build failed. Falling back to direct install..."
        sudo make -C compiler install
    fi
else
    echo "--- Installing Globally on macOS ---"
    # Ensure the install target exists in Makefile
    sudo make -C compiler install
fi

echo "--- SUCCESS! ---"
echo "The '${BINARY_NAME}' compiler has been installed globally."
echo "You can now run 'alg' from any terminal."
