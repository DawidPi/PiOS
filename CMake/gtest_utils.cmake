include(ExternalProject)

MACRO(enable_gtest sources dependencies)
    find_package(Threads)
    if (PiOS_BUILD_TESTS)
        enable_testing()
        include_directories(SYSTEM ${GTEST_INCLUDE} ${GMOCK_INCLUDE})
        include_directories(../sources)
        add_executable(${PROJECT_NAME} ${sources})
        target_link_libraries(${PROJECT_NAME} ${dependencies} Threads::Threads gtest gtest_main)
        add_dependencies(${PROJECT_NAME} ${dependencies} gtest gtest_main)
    endif ()
ENDMACRO()

FUNCTION(downloadGtest)
    ExternalProject_add(googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG master
            INSTALL_COMMAND ""

            SOURCE_DIR "${CMAKE_BINARY_DIR}/3rdParty/gtest-src"
            BINARY_DIR "${CMAKE_BINARY_DIR}/3rdParty/gtest-build"

            LOG_DOWNLOAD ON
            LOG_BUILD ON
            )

    add_library(gtest UNKNOWN IMPORTED)
    add_library(gtest_main UNKNOWN IMPORTED)
    add_library(gmock UNKNOWN IMPORTED)
    add_library(gmock_main UNKNOWN IMPORTED)
    add_dependencies(gtest googletest)
    add_dependencies(gtest_main googletest)
    add_dependencies(gmock googletest)
    add_dependencies(gmock_main googletest)
    ExternalProject_Get_Property(googletest binary_dir)
    set_target_properties(gtest PROPERTIES IMPORTED_LOCATION ${binary_dir}/googlemock/gtest/libgtest.a)
    set_target_properties(gtest_main PROPERTIES IMPORTED_LOCATION ${binary_dir}/googlemock/gtest/libgtest_main.a)
    set_target_properties(gmock PROPERTIES IMPORTED_LOCATION ${binary_dir}/googlemock/libgmock.a)
    set_target_properties(gmock_main PROPERTIES IMPORTED_LOCATION ${binary_dir}/googlemock/libgmock_main.a)

    ExternalProject_Get_Property(googletest source_dir)
    set(GTEST_INCLUDE ${source_dir}/googletest/include PARENT_SCOPE)
    set(GMOCK_INCLUDE ${source_dir}/googlemock/include PARENT_SCOPE)
ENDFUNCTION()