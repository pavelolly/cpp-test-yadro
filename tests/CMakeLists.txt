
add_executable(events events.cpp)
target_link_libraries(events PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(events)