cmake_minimum_required(VERSION 3.25)

project(ci VERSION 1.0.0 LANGUAGES CXX)

if(DEFINED VCPKG_INSTALLED_DIR)
    file(MAKE_DIRECTORY "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include" "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/include" )
endif()

find_package(wwa_opentelemetry_resource_detectors CONFIG REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(ci-cmake ../examples/example1.cpp)
target_link_libraries(ci-cmake PRIVATE wwa::opentelemetry::resource_detectors nlohmann_json::nlohmann_json)
target_compile_definitions(ci-cmake PRIVATE -DUSE_SYSTEM_INCLUDE)
set_target_properties(
    ci-cmake
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        COMPILE_DEFINITIONS WWA_OPENTELEMETRY_RESOURCE_DETECTORS_STATIC_DEFINE
)

if(WIN32 AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_definitions(ci-cmake PRIVATE -D_UNICODE)
    set_target_properties(ci-cmake PROPERTIES LINK_FLAGS "/ENTRY:wmainCRTStartup")
endif()

if(NOT WIN32)
    include(FindPkgConfig)
    pkg_check_modules(wwa_opentelemetry_resource_detectors REQUIRED IMPORTED_TARGET wwa_opentelemetry_resource_detectors)
    add_executable(ci-pkgconfig ../examples/example1.cpp)
    target_link_libraries(ci-pkgconfig PRIVATE PkgConfig::wwa_opentelemetry_resource_detectors nlohmann_json::nlohmann_json)
    target_compile_definitions(ci-pkgconfig PRIVATE -DUSE_SYSTEM_INCLUDE)
    set_target_properties(
        ci-pkgconfig
        PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO
            COMPILE_DEFINITIONS WWA_OPENTELEMETRY_RESOURCE_DETECTORS_STATIC_DEFINE
    )
endif()
