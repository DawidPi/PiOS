project(PC_simulation)

set(SOURCES main.cpp)

add_executable(${PROJECT_NAME} SOURCES)
add_dependencies(${PROJECT_NAME} PiOS)
target_link_libraries(${PROJECT_NAME} PiOS)