project(ARM_hardware)

set(SOURCES ../PiOS/tests/gtest_main.cpp ../PiOS/sources/tmp.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_dependencies(${PROJECT_NAME} PiOS)