project(Algorithms_tests)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        gtest_main.cpp
        FixedSizeBinaryTree_test.cpp
        SimpleBuddy_test.cpp
        SimpleBuddyFactory_test.cpp)

enable_gtest("${SOURCES}" Algorithms)

#target_link_libraries(${PROJECT_NAME} Algorithms)
#add_dependencies(${PROJECT_NAME} Algorithms)