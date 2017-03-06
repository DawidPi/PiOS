project(Algorithms)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        Buddy.hpp
        SimpleBuddy.cpp
        SimpleBuddy.hpp
        FixedSizeBinaryTreeImpl.hpp
        FixedSizeBinaryTree.hpp
        power.hpp
        FixedSizeBinaryTree.cpp
        SimpleBuddyFactory.cpp
        SimpleBuddyFactory.hpp)

add_library(${PROJECT_NAME} ${SOURCES})