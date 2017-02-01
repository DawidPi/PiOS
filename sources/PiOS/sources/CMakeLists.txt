project(PiOS)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        PiOS.hpp
        PiOS.cpp
        Scheduler.hpp
        # ThreadPool.hpp
        Thread.hpp)

add_library(${PROJECT_NAME} STATIC ${SOURCES})