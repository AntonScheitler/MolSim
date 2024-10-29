# make doc_doxygen optional if someone does not have / like doxygen
option(BUILD_DOCUMENTATION "Enable for building documentation with doxygen.")

find_package(Doxygen)
if (DOXYGEN_FOUND)
    if(BUILD_DOCUMENTATION)
        set(DOXYFILE_IN ${PROJECT_SOURCE_DIR}/Doxyfile)

        configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

        add_custom_target(doc_doxygen
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_IN}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                COMMENT "Building doxygen documentation"
                VERBATIM
        )
        set_target_properties(doc_doxygen
                PROPERTIES EXCLUDE_FROM_ALL TRUE
        )
    else(BUILD_DOCUMENTATION)
        message("building without document target")
    endif(BUILD_DOCUMENTATION)
else (DOXYGEN_FOUND)
    message("Doxygen is not installed")
endif (DOXYGEN_FOUND)
