set(CMAKE_SYSTEM_NAME      Darwin CACHE STRING "" FORCE)
set(CMAKE_SYSTEM_PROCESSOR x86_64 CACHE STRING "")
set(CMAKE_MACOSX_RPATH     ON     CACHE BOOL "")

set(triplet "${CMAKE_SYSTEM_PROCESSOR}-apple-macos10.6")

foreach(lang ASM C CXX)
    set(CMAKE_${lang}_COMPILER_TARGET "${CMAKE_SYSTEM_PROCESSOR}-apple-macos10.16" CACHE STRING "")
endforeach()

find_program(CMAKE_C_COMPILER   NAMES "${triplet}-clang" clang clang-20 clang-19 clang-18 clang-17 REQUIRED)
find_program(CMAKE_CXX_COMPILER NAMES "${triplet}-clang++" clang++ clang++-20 clang++-19 clang++-18 clang++-17 REQUIRED)
find_program(CMAKE_ASM_COMPILER NAMES "${triplet}-as" clang clang-20 clang-19 clang-18 clang-17 llvm-as llvm-as-20 llvm-as-19 llvm-as-18 llvm-as-17)
find_program(CMAKE_AR           NAMES "${triplet}-ar" llvm-ar llvm-ar-20 llvm-ar-19 llvm-ar-18 llvm-ar-17 ar)
find_program(CMAKE_RANLIB       NAMES "${triplet}-ranlib" llvm-ranlib llvm-ranlib-20 llvm-ranlib-19 llvm-ranlib-18 llvm-ranlib-17 ranlib)

string(APPEND CMAKE_C_FLAGS_INIT           " -fPIC ${VCPKG_C_FLAGS} ")
string(APPEND CMAKE_CXX_FLAGS_INIT         " -fPIC ${VCPKG_CXX_FLAGS} ")
string(APPEND CMAKE_C_FLAGS_DEBUG_INIT     " ${VCPKG_C_FLAGS_DEBUG} ")
string(APPEND CMAKE_CXX_FLAGS_DEBUG_INIT   " ${VCPKG_CXX_FLAGS_DEBUG} ")
string(APPEND CMAKE_C_FLAGS_RELEASE_INIT   " ${VCPKG_C_FLAGS_RELEASE} ")
string(APPEND CMAKE_CXX_FLAGS_RELEASE_INIT " ${VCPKG_CXX_FLAGS_RELEASE} ")

string(APPEND CMAKE_MODULE_LINKER_FLAGS_INIT " ${VCPKG_LINKER_FLAGS} ")
string(APPEND CMAKE_SHARED_LINKER_FLAGS_INIT " ${VCPKG_LINKER_FLAGS} ")
string(APPEND CMAKE_EXE_LINKER_FLAGS_INIT    " ${VCPKG_LINKER_FLAGS} ")

string(APPEND CMAKE_MODULE_LINKER_FLAGS_DEBUG_INIT   " ${VCPKG_LINKER_FLAGS_DEBUG} ")
string(APPEND CMAKE_SHARED_LINKER_FLAGS_DEBUG_INIT   " ${VCPKG_LINKER_FLAGS_DEBUG} ")
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG_INIT      " ${VCPKG_LINKER_FLAGS_DEBUG} ")
string(APPEND CMAKE_MODULE_LINKER_FLAGS_RELEASE_INIT " ${VCPKG_LINKER_FLAGS_RELEASE} ")
string(APPEND CMAKE_SHARED_LINKER_FLAGS_RELEASE_INIT " ${VCPKG_LINKER_FLAGS_RELEASE} ")
string(APPEND CMAKE_EXE_LINKER_FLAGS_RELEASE_INIT    " ${VCPKG_LINKER_FLAGS_RELEASE} ")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
