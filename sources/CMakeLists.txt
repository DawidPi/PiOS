include(../CMake/settings.cmake)
include(../CMake/gtest_utils.cmake)


if(PiOS_PLATFORM_TYPE STREQUAL "ARM")
    add_subdirectory(ARM)
elseif(PiOS_PLATFORM_TYPE STREQUAL "PC")
    add_subdirectory(PC_simulation)
elseif(PiOS_PLATFORM_TYPE STREQUAL "ALL")
    add_subdirectory(ARM)
    add_subdirectory(PC_simulation)
else()
    message(WARNING "No platform selected")
endif()

add_subdirectory(PiOS)