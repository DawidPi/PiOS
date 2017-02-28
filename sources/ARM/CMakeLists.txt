project(ARM_hardware)

set(SOURCES ../PiOS/tests/gtest_main.cpp ../Algorithms/sources/Buddy.cpp ../Algorithms/sources/Buddy.hpp)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_dependencies(${PROJECT_NAME} PiOS)