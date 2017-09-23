project(ContextAccess)

enable_language(ASM)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        StatusRegisterManagement.cpp
        VectorTableManagement.cpp
        BankedRegisters.cpp
        )

set(HEADERS
        StatusRegisterManagement.hpp
        VectorTableManagement.hpp
        BankedRegisters.hpp
        )

set(ASM
        registerAccess.S)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${ASM})
target_include_directories(${PROJECT_NAME} PUBLIC "../")
target_link_libraries(${PROJECT_NAME} ${DEPENDENCIES})
set_source_files_properties(${SOURCES} ${HEADERS} PROPERTIES COMPILE_FLAGS "-Weffc++")