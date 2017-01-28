MACRO(enable_gtest sources)
    find_package(GTest)
    if(GTEST_FOUND)
        include_directories(../sources ${GTEST_INCLUDE_DIRS})
        add_executable(${PROJECT_NAME} ${sources})
        target_link_libraries(${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})
        add_test(PiOS_Tests ${PROJECT_NAME})
    endif()
ENDMACRO()