project(ARM_hardware)

set(SOURCES ../PiOS/tests/gtest_main.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCES})