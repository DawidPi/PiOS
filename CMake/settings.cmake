set(PiOS_PLATFORM_TYPE "ALL" CACHE STRING "type of platform, on which OS is going to be run on" FORCE)
set_property(CACHE PiOS_PLATFORM_TYPE PROPERTY STRINGS ARM PC ALL UNKNOWN)
option(PiOS_BUILD_TESTS "Builds tests using gtest" ON)