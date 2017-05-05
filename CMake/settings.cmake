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
option(PiOS_BUILD_EXAMPLES "Builds PiOS examples" ON)
option(PiOS_BUILD_NEW_OPERATOR "Build library with custom operator new, which uses PiOS dynamic allocation" ON)