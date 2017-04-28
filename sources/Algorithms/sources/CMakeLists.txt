project(Algorithms)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        SimpleBuddy.cpp
        SimpleBuddy.hpp
        FixedSizeBinaryTreeImpl.hpp
        FixedSizeBinaryTree.hpp
        power.hpp
        FixedSizeBinaryTree.cpp
        SimpleBuddyFactory.cpp
        SimpleBuddyFactory.hpp
        EDF.hpp
        ListEDF.cpp
        ListEDF.hpp
        TLSFImpl.hpp
        TLSF.hpp
        MsbLsb.hpp
        BitSlice.cpp
        BitSlice.hpp)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ../../PiOS/sources)
target_link_libraries(${PROJECT_NAME} PiOS)
target_compile_options(${PROJECT_NAME} PRIVATE "-Weffc++")