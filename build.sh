#!/bin/bash
set -e

PKGDIR="algocompiler-pkg"
PKGNAME="algocompiler"
VERSION="1.0.0"

detect_arch() {
  if command -v dpkg >/dev/null 2>&1; then
    dpkg --print-architecture
    return
  fi

  case "$(uname -m)" in
    x86_64) echo "amd64" ;;
    aarch64|arm64) echo "arm64" ;;
    *) echo "amd64" ;;
  esac
}

ARCH="$(detect_arch)"
DEB="${PKGNAME}_${VERSION}_${ARCH}.deb"

if ! command -v dpkg-deb >/dev/null 2>&1; then
  echo "Error: dpkg-deb not found. Install it on Debian/Ubuntu with:"
  echo "  sudo apt update && sudo apt install -y dpkg-dev"
  exit 1
fi

echo "--- Building ${PKGNAME} ---"
make clean
make

echo "--- Preparing package directory ---"
rm -rf "${PKGDIR}/usr/bin"
mkdir -p "${PKGDIR}/usr/bin"

cp -f "./algocompiler" "${PKGDIR}/usr/bin/algocompiler"
chmod 755 "${PKGDIR}/usr/bin/algocompiler"
chmod 755 "${PKGDIR}/DEBIAN/postinst" "${PKGDIR}/DEBIAN/prerm"

echo "--- Writing control file (Architecture=${ARCH}) ---"
tmp_control="$(mktemp)"
sed "s/^Architecture: .*/Architecture: ${ARCH}/" "${PKGDIR}/DEBIAN/control" > "${tmp_control}"
mv "${tmp_control}" "${PKGDIR}/DEBIAN/control"

echo "--- Building .deb ---"
dpkg-deb --build "${PKGDIR}" "${DEB}"

abs_deb="$(cd "$(dirname "${DEB}")" && pwd)/$(basename "${DEB}")"
echo "Built: ${abs_deb}"
echo "Install on Debian/Ubuntu with:"
echo "  sudo apt install ./${DEB}"

