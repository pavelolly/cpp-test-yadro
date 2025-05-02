
add_library(${PROJECT_NAME}-main
    body.cpp event.cpp utils/timestamp.cpp)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}-main)