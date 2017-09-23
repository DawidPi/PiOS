project(Timer)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        Timer.cpp)

set(HEADERS
        Timer.hpp
        TimerRegisters.hpp)

set(DEPENDENCIES
        ContextAccess)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${DEPENDENCIES})
target_include_directories(${PROJECT_NAME} PUBLIC "../")
target_include_directories(${PROJECT_NAME} PUBLIC "../ContextAccess")
set_source_files_properties(${SOURCES} ${HEADERS} PROPERTIES COMPILE_FLAGS "-Weffc++")