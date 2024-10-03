#!/bin/sh

# dependencies: powershell, mingw-w64
# may need to set WINEPATH="/usr/x86_64-w64-mingw32/lib;/usr/lib/gcc/x86_64-w64-mingw32/13-win32/" to run executables

SCRIPT_DIR="$(cd "$(dirname "${0}")" && pwd)"
TARGET=x64-mingw

# shellcheck source=./cross-compile-common.sh
. "${SCRIPT_DIR}/cross-compile-common.sh"
