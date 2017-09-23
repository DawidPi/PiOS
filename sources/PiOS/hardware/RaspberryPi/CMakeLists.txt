add_subdirectory(Uart)
add_subdirectory(Timer)
add_subdirectory(Runtime)
add_subdirectory(ContextAccess)

set(CMAKE_CXX_STANDARD 14)

enable_language(ASM)

set(LINKER_SCRIPT memmap)
set(LINKER_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_SCRIPT}")


set(SOURCES
        startup.cpp
        memmap
        interruptRoutines.cpp
        Context.cpp
        SystemCall.cpp
        Logger.cpp
        )

set(HEADERS
        Register.hpp
        Context.hpp
        SystemCall.hpp
        Logger.hpp
        )

set(ASM_SOURCES
        startup.S
        interrupts.S)

set(DEPENDENCIES
        PiOS
        Algorithms
        AllocationOperators
        Uart
        Timer
        Runtime
        ContextAccess
        )


add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS} ${ASM_SOURCES})
set_source_files_properties(${SOURCES} ${HEADERS} PROPERTIES COMPILE_FLAGS "-Weffc++")
target_include_directories(${PROJECT_NAME} PUBLIC "./")
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEPENDENCIES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LINKER_FLAGS})