# make doc_doxygen optional if someone does not have / like doxygen

find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYFILE_IN ${PROJECT_SOURCE_DIR}/Doxyfile)

    configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

    add_custom_target(doc_doxygen
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_IN}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Building doxygen documentation"
            VERBATIM
    )
else (DOXYGEN_FOUND)
    message("Doxygen is not installed")
endif (DOXYGEN_FOUND)