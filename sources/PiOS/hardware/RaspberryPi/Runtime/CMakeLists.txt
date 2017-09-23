project(Runtime)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        dsoHandle.cpp
        sysCalls.cpp)

set(DEPENDENCIES
        Uart)

add_library(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${DEPENDENCIES})
set_source_files_properties(${SOURCES} PROPERTIES COMPILE_FLAGS "-Weffc++")