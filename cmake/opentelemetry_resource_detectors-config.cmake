get_filename_component(OTEL_RESOURCE_DETECTORS_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

list(APPEND CMAKE_MODULE_PATH ${OTEL_RESOURCE_DETECTORS_CMAKE_DIR})

include(CMakeFindDependencyMacro)
find_dependency(opentelemetry-cpp QUIET REQUIRED COMPONENTS resources)

if(NOT TARGET opentelemetry_resource_detectors::opentelemetry_resource_detectors)
    include("${OTEL_RESOURCE_DETECTORS_CMAKE_DIR}/opentelemetry_resource_detectors-target.cmake")
endif()

set(OTEL_RESOURCE_DETECTORS_LIBRARIES opentelemetry_resource_detectors::opentelemetry_resource_detectors)
