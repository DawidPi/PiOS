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
        SimpleBuddyFactory.hpp
        EDF.hpp
        RealTimeTask.cpp
        RealTimeTask.hpp
        Task.cpp
        Task.hpp
        Context.cpp
        Context.hpp
        ListEDF.cpp
        ListEDF.hpp
        Time.cpp
        Time.hpp)

add_library(${PROJECT_NAME} ${SOURCES})