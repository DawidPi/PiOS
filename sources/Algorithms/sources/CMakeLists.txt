project(Algorithms)

set(CMAKE_CXX_STANDARD 14)

set(EXTERNAL
        ../external/TLSF-2.4.6/src/tlsf.c
        ../external/TLSF-2.4.6/src/tlsf.h)

set(SOURCES
        SimpleBuddy.cpp
        FixedSizeBinaryTree.cpp
        SimpleBuddyFactory.cpp
        ListEDF.cpp
        BitSlice.cpp
        TLSFExternal.cpp)

set(HEADERS
        SimpleBuddy.hpp
        FixedSizeBinaryTreeImpl.hpp
        FixedSizeBinaryTree.hpp
        power.hpp
        SimpleBuddyFactory.hpp
        EDF.hpp
        ListEDF.hpp
        TLSFImpl.hpp
        TLSF.hpp
        MsbLsb.hpp
        BitSlice.hpp
        TLSFExternal.hpp)

add_library(${PROJECT_NAME} STATIC ${EXTERNAL} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC "./")
target_include_directories(${PROJECT_NAME} PRIVATE "../external/TLSF-2.4.6/src")
target_link_libraries(${PROJECT_NAME} PUBLIC PiOS)
target_compile_options(${PROJECT_NAME} PRIVATE "-Weffc++")

install(FILES ${HEADERS} DESTINATION include/PiOS/Algorithms/)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)