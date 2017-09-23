project(Uart)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        OutputEngine.cpp
        Uart.cpp
        c_uart.cpp)

set(HEADERS
        OutputEngine.hpp
        Uart.hpp
        c_uart.hpp)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC "../")
set_source_files_properties(${SOURCES} ${HEADERS} PROPERTIES COMPILE_FLAGS "-Weffc++")