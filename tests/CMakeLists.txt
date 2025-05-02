
add_executable(events events.cpp)
target_link_libraries(events PRIVATE GTest::gtest ${PROJECT_NAME}-main)
target_include_directories(events PRIVATE "../src")
enable_warnings(events)

add_executable(string_utils string_utils.cpp)
target_link_libraries(string_utils PRIVATE GTest::gtest ${PROJECT_NAME}-main)
target_include_directories(string_utils PRIVATE "../src")
enable_warnings(events)

include(GoogleTest)
gtest_discover_tests(events)
gtest_discover_tests(string_utils)