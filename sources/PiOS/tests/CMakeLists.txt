project(PiOS_tests)

set(CMAKE_CXX_STANDARD 14)
set(SOURCES gtest_main.cpp)

set(SOURCES
        gtest_main.cpp
        Time_test.cpp
        PiOS_test.cpp)

enable_gtest("${SOURCES}" PiOS)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--wrap,malloc")