cmake_minimum_required(VERSION 3.22)
project(cpp_test_yadro CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

function(enable_warnings target)
    if (MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive- /w34062) # /w34062 == enable warning 4062 (-Wswitch-enum) for level W3
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Wswitch -Wswitch-enum) 
    endif()
endfunction()

add_subdirectory(src)
add_subdirectory(tests/generator)

option(BUILD_UNIT_TESTS "Build unit-tests" OFF)

if (BUILD_UNIT_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)
    add_subdirectory(tests/unit-tests)
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION .)

file(GLOB TEST_FILES "tests/data/*.dat" "tests/data/*.ans")
install(FILES ${TEST_FILES} DESTINATION data/)
