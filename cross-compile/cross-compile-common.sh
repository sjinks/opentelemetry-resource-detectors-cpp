#!/bin/sh

: "${SCRIPT_DIR:?SCRIPT_DIR must be set}"
: "${TARGET:?TARGET must be set}"

cmake -B "${SCRIPT_DIR}/../build-${TARGET}" -S "${SCRIPT_DIR}/../" \
	-DCMAKE_TOOLCHAIN_FILE="${SCRIPT_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake" \
	-DVCPKG_TARGET_TRIPLET="${TARGET}-cross" \
	-DVCPKG_OVERLAY_TRIPLETS=triplets \
	-DVCPKG_CHAINLOAD_TOOLCHAIN_FILE="${SCRIPT_DIR}/toolchains/${TARGET}-cross-toolchain.cmake"

cmake --build "${SCRIPT_DIR}/../build-${TARGET}"
