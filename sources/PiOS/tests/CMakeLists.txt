project(PiOS_tests)

set(CMAKE_CXX_STANDARD 14)
set(SOURCES gtest_main.cpp ThreadTest.cpp)

enable_gtest("${SOURCES}")

target_link_libraries(${PROJECT_NAME} PiOS)