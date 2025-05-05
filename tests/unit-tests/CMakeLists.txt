
include(GoogleTest)

set(TESTS events string_utils load analyzer)

foreach(test ${TESTS})
    add_executable(${test} ${test}.cpp)
    target_link_libraries(${test} PRIVATE GTest::gtest ${PROJECT_NAME}-main)
    target_include_directories(${test} PRIVATE "../../src")
    enable_warnings(${test})
    gtest_discover_tests(${test})
endforeach()