get_filename_component(OTEL_RESOURCE_DETECTORS_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

list(APPEND CMAKE_MODULE_PATH ${OTEL_RESOURCE_DETECTORS_CMAKE_DIR})

include(CMakeFindDependencyMacro)
find_dependency(opentelemetry-cpp QUIET REQUIRED COMPONENTS sdk)

if(NOT TARGET wwa_opentelemetry_resource_detectors)
    include("${OTEL_RESOURCE_DETECTORS_CMAKE_DIR}/wwa_opentelemetry_resource_detectors-target.cmake")
    add_library(wwa::opentelemetry::resource_detectors ALIAS wwa_opentelemetry_resource_detectors)
endif()
