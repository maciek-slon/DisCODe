# =======================================================================
# Doxygen support
# =======================================================================

find_package(Doxygen)
if(NOT DOXYGEN_EXECUTABLE)
  message(STATUS "WARNING: Doxygen not found so documentation not generated.")
endif(NOT DOXYGEN_EXECUTABLE)

#
# Generating documentation with doxygen
#
if(DOXYGEN_EXECUTABLE)

    # Set html output directory location
    SET(HTML_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/doc/fradia.html)

    # N.B. Both the following custom rules assume the doc directory exists
    # at make time, and the following install(DIRECTORY... must have doc exist
    # at cmake time.  Therefore, create the doc directory at CMake time.
    # (Linux experimentation indicates this is a no-op if the empty or
    # non-empty directory already exists.)
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/doc/fradia.html)

    ADD_CUSTOM_TARGET(
        doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/doc_src/FraDIA.Doxyfile
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/doc_src/fradia.png ${HTML_OUTPUT_DIR}/fradia.png
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        DEPENDS ${CMAKE_SOURCE_DIR}/doc_src/FraDIA.Doxyfile
    )

endif(DOXYGEN_EXECUTABLE)
