project(Algorithms_tests)

set(CMAKE_CXX_STANDARD 14)
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCES
        gtest_main.cpp
        FixedSizeBinaryTree_test.cpp
        SimpleBuddy_test.cpp
        Time_test.cpp
        ListEDFTest.cpp
        MSBLSBCalculator_test.cpp
        BitSliceCalculator_test.cpp
        TLSF_test.cpp)

enable_gtest("${SOURCES}" Algorithms)

#target_link_libraries(${PROJECT_NAME} Algorithms)
#add_dependencies(${PROJECT_NAME} Algorithms)