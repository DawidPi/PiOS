project(Algorithms)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        Buddy.hpp
        Buddy.cpp
        SimpleBuddy.cpp
        SimpleBuddy.hpp
        FixedSizeBinaryTreeImpl.hpp
        FixedSizeBinaryTree.hpp
        power.hpp FixedSizeBinaryTree.cpp)

add_library(${PROJECT_NAME} ${SOURCES})