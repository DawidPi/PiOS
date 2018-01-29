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
        TLSFExternal.hpp
        List.hpp)

set(EXTERNAL_LIB TLSF_EXTERNAL)
add_library(${EXTERNAL_LIB} ${EXTERNAL})
target_include_directories(${EXTERNAL_LIB} PUBLIC "../external/TLSF-2.4.6/src/")

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC "./")
target_link_libraries(${PROJECT_NAME} PUBLIC PiOS ${EXTERNAL_LIB})
target_compile_options(${PROJECT_NAME} PRIVATE "-Weffc++")

install(FILES ${HEADERS} DESTINATION include/PiOS/Algorithms/)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)