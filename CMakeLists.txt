cmake_minimum_required(VERSION 3.2.2)
include(CMake/doxygen.cmake)

set(VERSION_MAJOR 0 CACHE STRING "Project major version number.")
set(VERSION_MINOR 0 CACHE STRING "Project minor version number.")
set(VERSION_PATCH 1 CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

add_subdirectory(sources)
enable_doxygen()

if (PiOS_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()