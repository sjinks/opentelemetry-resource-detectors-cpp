#!/bin/sh

set -e

SCRIPT_DIR="$(cd "$(dirname "${0}")" && pwd)"
TARGET=x86-linux

# shellcheck source=./cross-compile-common.sh
. "${SCRIPT_DIR}/cross-compile-common.sh"
