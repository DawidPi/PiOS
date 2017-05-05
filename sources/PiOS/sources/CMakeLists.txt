project(PiOS)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        Pi.cpp
        Task.cpp
        RealTimeTask.cpp
        Time.cpp
        Scheduler.cpp)

set(HEADERS
        Pi.hpp
        Task.hpp
        RealTimeTask.hpp
        Time.hpp
        DynamicAllocator.hpp
        Scheduler.hpp
        ContextInterface.hpp CommonTypes.hpp ../tests/Mocks.hpp)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} hardware)
target_include_directories(${PROJECT_NAME} PUBLIC "../hardware/${PiOS_PLATFORM_TYPE}")
target_compile_options(${PROJECT_NAME} PRIVATE "-Weffc++")

install(FILES ${HEADERS} DESTINATION include/PiOS)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)