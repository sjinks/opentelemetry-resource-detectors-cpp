#!/bin/sh

set -e

# dependencies: powershell, mingw-w64
# may need to set WINEPATH="/usr/i686-w64-mingw32/lib;/usr/lib/gcc/i686-w64-mingw32/13-win32/" to run executables

SCRIPT_DIR="$(cd "$(dirname "${0}")" && pwd)"
TARGET=x86-mingw

# shellcheck source=./cross-compile-common.sh
. "${SCRIPT_DIR}/cross-compile-common.sh"
