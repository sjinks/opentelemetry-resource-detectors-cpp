add_executable(example1 example1.cpp)
target_link_libraries(example1 PRIVATE ${PROJECT_NAME} nlohmann_json::nlohmann_json)
