
add_library(${PROJECT_NAME}-main
    body.cpp event.cpp club_info_analyzer.cpp utils/timestamp.cpp utils/string.cpp)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}-main)

enable_warnings(${PROJECT_NAME}-main)
enable_warnings(${PROJECT_NAME})