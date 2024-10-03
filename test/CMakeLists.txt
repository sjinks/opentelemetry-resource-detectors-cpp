add_executable(
    test_container_utils
    container_utils/extract_container_id_v1.cpp
    container_utils/extract_container_id_v2.cpp
)
target_link_libraries(test_container_utils PRIVATE helpers GTest::gtest_main)

add_executable(
    test_string_utils
    string_utils/ends_with.cpp
    string_utils/nth_word.cpp
    string_utils/split.cpp
    string_utils/starts_with.cpp
)
target_link_libraries(test_string_utils PRIVATE helpers GTest::gtest_main)

add_executable(
    test_os_utils
    os_utils/get_host_arch.cpp
    os_utils/get_os_type.cpp
)
target_link_libraries(test_os_utils PRIVATE helpers GTest::gtest_main)

add_executable(
    test_process_utils
    process_utils/parse_zero_delimited_stream.cpp
)
target_link_libraries(test_process_utils PRIVATE helpers GTest::gtest_main)

set_target_properties(
    test_container_utils test_string_utils test_os_utils test_process_utils
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        COMPILE_DEFINITIONS WWA_OPENTELEMETRY_RESOURCE_DETECTORS_STATIC_DEFINE
)

include(GoogleTest)
gtest_discover_tests(test_string_utils)
gtest_discover_tests(test_container_utils)
gtest_discover_tests(test_os_utils)
gtest_discover_tests(test_process_utils)
