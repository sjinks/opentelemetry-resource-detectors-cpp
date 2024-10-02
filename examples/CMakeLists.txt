add_executable(example1 example1.cpp)
target_link_libraries(example1 PRIVATE ${PROJECT_NAME} nlohmann_json::nlohmann_json)

add_executable(example2 example2.cpp)
target_link_libraries(
    example2
    PRIVATE
        ${PROJECT_NAME}
        opentelemetry-cpp::ostream_span_exporter
        nlohmann_json::nlohmann_json
)

set_target_properties(
    example1 example2
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        COMPILE_DEFINITIONS WWA_OPENTELEMETRY_RESOURCE_DETECTORS_STATIC_DEFINE
)
