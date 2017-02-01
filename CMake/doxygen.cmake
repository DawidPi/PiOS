MACRO(enable_doxygen)
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        message("Doxygen available")
        set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen/PiOS.doxygen)
        set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/docs/doxygen/doxyfile)
        configure_file(${doxyfile_in} ${doxyfile} @ONLY)

        add_custom_target(doxygen ALL
                COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs
                COMMENT Creating doxygen documentation.)
    endif ()
ENDMACRO()