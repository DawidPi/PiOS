project(Algorithms)

set(CMAKE_CXX_STANDARD 14)

#todo implement your own TLSF, as this one is on virus GPL licence
set(EXTERNAL
        ../external/TLSF-2.4.6/src/tlsf.c
        ../external/TLSF-2.4.6/src/tlsf.h)

set(SOURCES
        ListEDF.cpp
        BitSlice.cpp
        TLSFExternal.cpp)

set(HEADERS
        power.hpp
        EDF.hpp
        ListEDF.hpp
        TLSFImpl.hpp
        TLSF.hpp
        MsbLsb.hpp
        BitSlice.hpp
        TLSFExternal.hpp)

# adding external tlsf library implementation
add_library(${PROJECT_NAME}_TLSF STATIC ${EXTERNAL})
target_include_directories(${PROJECT_NAME}_TLSF PUBLIC "../external/TLSF-2.4.6/src")

# create new library
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC "./")
target_link_libraries(${PROJECT_NAME} PUBLIC PiOS ${PROJECT_NAME}_TLSF)
target_compile_options(${PROJECT_NAME} PRIVATE "-Weffc++")
set_source_files_properties(${EXTERNAL} COMPILE_FLAGS "${COMPILER_FLAGS_C}")

install(FILES ${HEADERS} DESTINATION include/PiOS/Algorithms/)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
