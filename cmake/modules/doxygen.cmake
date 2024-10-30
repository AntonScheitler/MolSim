# enable the optional DOC_TARGET option for creating a make target "doc_doxygen" for building the documentation
# using doxygen
option(DOC_TARGET "Enable for creating make target for building documentation with doxygen.")

if(DOC_TARGET)
    find_package(Doxygen)

    if (DOXYGEN_FOUND)

        message("Building with doxygen documentation generation target")

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

    else (DOXYGEN_FOUND)
        message("Doxygen is not installed")
    endif (DOXYGEN_FOUND)

else(DOC_TARGET)
    message("Building without doxygen documentation generation target")
endif(DOC_TARGET)