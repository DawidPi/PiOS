project(Algorithms_tests)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        gtest_main.cpp
        FixedSizeBinaryTree_test.cpp
        SimpleBuddy_test.cpp
        ListEDFTest.cpp
        MSBLSBCalculator_test.cpp
        BitSliceCalculator_test.cpp
        TLSF_test.cpp)

enable_gtest("${SOURCES}" Algorithms)