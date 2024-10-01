cmake_minimum_required(VERSION 3.25)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
    wwa_opentelemetry_resource_detectors
    VERSION 1.0.0
    DESCRIPTION "Resource Detectors for OpenTelemetry"
    HOMEPAGE_URL "https://github.com/sjinks/opentelemetry-resource-detectors-cpp"
    LANGUAGES CXX
)

set(CMAKE_VERBOSE_MAKEFILE ON)

option(WITH_TESTING "Whether to enable tests" ON)
option(WITH_EXAMPLES "Whether to enable examples" ON)
option(INSTALL_RESOURCE_DETECTORS "Whether to install Resource Detectors" ON)

find_package(opentelemetry-cpp CONFIG REQUIRED)

add_library(utils OBJECT)
target_sources(
    utils
    PUBLIC
        src/container_utils.cpp
        src/os_utils.cpp
        src/process_utils.cpp
        src/string_utils.cpp
)

set_target_properties(
    utils
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        POSITION_INDEPENDENT_CODE ON
)

target_link_libraries(utils PUBLIC opentelemetry-cpp::resources)

add_library(helpers STATIC $<TARGET_OBJECTS:utils>)
target_include_directories(helpers PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_library(${PROJECT_NAME})

set(
    sources
        src/container_resource_detector.cpp
        src/os_resource_detector.cpp
        src/process_resource_detector.cpp
)

set(
    headers
        include/opentelemetry/resource/wwa/export.h
        include/opentelemetry/resource/wwa/container_resource_detector.h
        include/opentelemetry/resource/wwa/os_resource_detector.h
        include/opentelemetry/resource/wwa/process_resource_detector.h
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE
        ${sources}
        $<TARGET_OBJECTS:utils>
)

target_link_libraries(${PROJECT_NAME} PUBLIC opentelemetry-cpp::resources)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
        POSITION_INDEPENDENT_CODE ON
        PUBLIC_HEADER "${headers}"
        INTERFACE_COMPILE_FEATURES cxx_std_17
)

if(NOT BUILD_SHARED_LIBS)
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -DOPENTELEMETRY_RESOURCE_DETECTORS_STATIC_DEFINE)
endif()

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

if(WITH_TESTING)
    include(FindGTest)
    find_package(GTest CONFIG)
    if(TARGET GTest::gtest)
        enable_testing()
        add_subdirectory(test)
    else()
        message(WARNING "GTest not found, tests will not be built")
    endif()
endif()

if(WITH_EXAMPLES)
    find_package(nlohmann_json)
    if(TARGET nlohmann_json::nlohmann_json)
        add_subdirectory(examples)
    else()
        message(WARNING "nlohmann_json not found, examples will not be built")
    endif()
endif()

if(INSTALL_RESOURCE_DETECTORS)
    include(CMakePackageConfigHelpers)
    include(GNUInstallDirs)

    install(
        TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}-target
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/opentelemetry/resource/wwa"
    )

    install(
        EXPORT ${PROJECT_NAME}-target
        FILE ${PROJECT_NAME}-target.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
    )

    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
        COPYONLY
    )

    install(
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )

    if(BUILD_SHARED_LIBS)
        set(REQUIRES_SUFFIX ".private")
    endif()
    configure_file("${CMAKE_SOURCE_DIR}/cmake/pkg-config.pc.in" "${PROJECT_NAME}.pc" @ONLY)
    install(
        FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
    )
endif()
