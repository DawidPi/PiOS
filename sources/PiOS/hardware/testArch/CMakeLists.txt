set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        Context.cpp)

set(HEADERS
        Context.hpp SystemCall.cpp SystemCall.hpp)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})





