project(PiOS)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES tmp.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCES})