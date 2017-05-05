project(AllocationOperators_tests)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        gtest_main.cpp
        CustomNewDeleteTest.cpp
        )

set(DEPENDENCIES
        AllocationOperators)

enable_gtest("${SOURCES}" "${DEPENDENCIES}")
target_include_directories(${PROJECT_NAME} PUBLIC "../../PiOS/tests/")