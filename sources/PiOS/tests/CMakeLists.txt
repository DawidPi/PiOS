project(PiOS_tests)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        gtest_main.cpp
        TimeTest.cpp
        PiOSTest.cpp
        Mocks.hpp
        PiOSHolderTest.cpp
        TaskTest.cpp
        SchedulerTest.cpp)

enable_gtest("${SOURCES}" PiOS)