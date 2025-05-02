
add_executable(events events.cpp)
target_link_libraries(events PRIVATE GTest::gtest ${PROJECT_NAME}-main)
target_include_directories(events PRIVATE "../src")

include(GoogleTest)
gtest_discover_tests(events)