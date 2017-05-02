project(Algorithms)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        SimpleBuddy.cpp
        FixedSizeBinaryTree.cpp
        SimpleBuddyFactory.cpp
        ListEDF.cpp
        BitSlice.cpp)

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
        BitSlice.hpp)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ../../PiOS/sources)
target_link_libraries(${PROJECT_NAME} PiOS)
target_compile_options(${PROJECT_NAME} PRIVATE "-Weffc++")

install(FILES ${HEADERS} DESTINATION include/PiOS/Algorithms/)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)