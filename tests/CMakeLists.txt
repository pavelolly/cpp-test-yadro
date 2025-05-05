
add_executable(generator generator.cpp)
target_include_directories(generator PRIVATE "../src")
target_link_libraries(generator PRIVATE ${PROJECT_NAME}-main)