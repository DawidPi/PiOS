project(PiOS)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        PiOS.cpp
        PiOS.hpp
        Task.hpp
        Task.cpp
        RealTimeTask.cpp
        RealTimeTask.hpp
        Time.cpp
        Time.hpp
        DynamicAllocator.hpp
        Scheduler.hpp
        dynAllocation.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_compile_options(${PROJECT_NAME} PRIVATE "-Weffc++")