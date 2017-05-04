project(Algorithms_tests)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        gtest_main.cpp
        FixedSizeBinaryTreeTest.cpp
        SimpleBuddyTest.cpp
        ListEDFTest.cpp
        MSBLSBCalculatorTest.cpp
        BitSliceCalculatorTest.cpp
        TLSFTest.cpp)

enable_gtest("${SOURCES}" Algorithms)