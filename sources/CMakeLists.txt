include(../CMake/settings.cmake)
include(../CMake/gtest_utils.cmake)

if(PiOS_BUILD_TESTS)
    downloadGtest()
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

add_subdirectory(Algorithms)
add_subdirectory(PiOS)