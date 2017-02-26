project(Algorithms_tests)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES Buddy_test.cpp gtest_main.cpp FixedSizeBinaryTree_test.cpp)

enable_gtest("${SOURCES}" ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} Algorithms)
add_dependencies(${PROJECT_NAME} Algorithms)