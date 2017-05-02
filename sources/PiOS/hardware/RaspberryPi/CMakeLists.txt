set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        Context.cpp)

set(HEADERS
        Context.hpp)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
