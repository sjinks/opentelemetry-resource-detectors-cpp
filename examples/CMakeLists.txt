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
