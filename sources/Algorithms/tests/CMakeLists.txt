project(Algorithms_tests)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        gtest_main.cpp
        ListEDFTest.cpp
        MSBLSBCalculatorTest.cpp
        BitSliceCalculatorTest.cpp
        TLSFTest.cpp
        PowerTest.cpp
        ListTest.cpp)

include_directories(SYSTEM ${GTEST_INCLUDE} ${GMOCK_INCLUDE})
enable_gtest("${SOURCES}" Algorithms)