set(CMAKE_SYSTEM_NAME Generic)

set(COMPILER_NAME arm-none-eabi)

set(CMAKE_C_COMPILER ${COMPILER_NAME}-gcc CACHE STRING "path to arm C compiler" FORCE)
set(CMAKE_CXX_COMPILER ${COMPILER_NAME}-g++ CACHE STRING "path to arm C++ compiler" FORCE)
set(CMAKE_ASM_COMPILER ${COMPILER_NAME}-as CACHE STRING "path to arm assembler compiler" FORCE)
set(CMAKE_AR "${COMPILER_NAME}-ar" CACHE STRING "path to arm ar" FORCE)
set(CMAKE_NM "${COMPILER_NAME}-nm" CACHE STRING "path to arm nm" FORCE)
set(CMAKE_LINKER "${COMPILER_NAME}-ld" CACHE STRING "path to arm linker" FORCE)
set(CMAKE_OBJCOPY "${COMPILER_NAME}-objcopy" CACHE STRING "path to arm objcopy program" FORCE)
set(CMAKE_OBJDUMP "${COMPILER_NAME}-objdump" CACHE STRING "path to arm objdump program" FORCE)
set(CMAKE_STRIP "${COMPILER_NAME}-strip" CACHE STRING "path to arm strip program" FORCE)

set(COMPILER_NO_OS_FLAGS
        "-nostartfiles -ffreestanding -Wall")

set(COMPILER_NO_OS_FLAGS_NOSTDLIB
        "-nostdlib -nostartfiles -ffreestanding -nodefaultlibs -Wall")

set(COMPILER_NO_RTTI_EXCEPTIONS_FLAGS
        "-fno-rtti -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables")

set(COMPILER_DEAD_CODE_REMOVAL_FLAGS
        "-fdata-sections -ffunction-sections -Wl,--gc-sections")

#todo because of using exceptions and defining own exception functions -Os must be always enabled for compilation to succeed
set(COMPILER_OPTIMIZATIONS
        "-Os")

set(COMPILER_CPP_SUPPORT
        "")

#for cortex-a7 (RaspberryPi2) note -mfloat-abi=hard can be used as well
set(COMPILER_ARCH_FLAGS
        "-marm -mfloat-abi=soft")

set(COMPILER_FLAGS_CPP
        "${COMPILER_NO_OS_FLAGS} ${COMPILER_CPP_SUPPORT} ${COMPILER_ARCH_FLAGS} ${COMPILER_NO_RTTI_EXCEPTIONS_FLAGS} ${COMPILER_DEAD_CODE_REMOVAL_FLAGS}")

set(COMPILER_FLAGS
        "${COMPILER_NO_OS_FLAGS} ${COMPILER_CPP_SUPPORT} ${COMPILER_ARCH_FLAGS} ${COMPILER_DEAD_CODE_REMOVAL_FLAGS}")

set(CMAKE_C_FLAGS_RELEASE "-DNDEBUG ${COMPILER_OPTIMIZATIONS}" CACHE STRING "compile flags for C language" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG ${COMPILER_OPTIMIZATIONS}" CACHE STRING "compile flags for C++ language" FORCE)
set(CMAKE_ASM_FLAGS_RELEASE "" CACHE STRING "compile flags for ASM language" FORCE)

set(CMAKE_C_FLAGS_DEBUG "-g" CACHE STRING "compile flags for C language" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "-g" CACHE STRING "compile flags for C++ language" FORCE)
set(CMAKE_ASM_FLAGS_DEBUG "" CACHE STRING "compile flags for ASM language" FORCE)

set(CMAKE_C_FLAGS "${COMPILER_FLAGS}" CACHE STRING "common compile flags for C language" FORCE)
set(CMAKE_CXX_FLAGS "${COMPILER_FLAGS_CPP}" CACHE STRING "common compile flags for C++ language" FORCE)
set(CMAKE_ASM_FLAGS " " CACHE STRING "common compile flags for ASM language" FORCE)

#do not search path
set(CMAKE_FIND_ROOT_PATH "")
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)