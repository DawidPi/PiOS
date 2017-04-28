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
        ContextInterface.hpp)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} hardware)
target_include_directories(${PROJECT_NAME} PUBLIC "../hardware/${PiOS_PLATFORM_TYPE}")
target_compile_options(${PROJECT_NAME} PRIVATE "-Weffc++")