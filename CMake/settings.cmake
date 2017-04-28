set(PiOS_PLATFORM_TYPE "testArch" CACHE STRING "type of platform, on which OS is going to be run on" FORCE)


#thanks to refaim from StackOverflow
MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET(dirlist "")
    FOREACH (child ${children})
        IF (IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${child})
        ENDIF ()
    ENDFOREACH ()
    SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(AVAILABLE_PLATFLORMS "${CMAKE_SOURCE_DIR}/sources/PiOS/hardware")

set_property(CACHE PiOS_PLATFORM_TYPE PROPERTY STRINGS ${AVAILABLE_PLATFLORMS})
option(PiOS_BUILD_TESTS "Builds tests using gtest" ON)