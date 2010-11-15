# =======================================================================
# Doxygen support
# =======================================================================

find_package(Doxygen)

#
# Generating documentation with doxygen
#
if(DOXYGEN_EXECUTABLE)

    # Set html output directory location
    SET(HTML_OUTPUT_DIR ${CMAKE_BINARY_DIR}/doc/html)

    # N.B. Both the following custom rules assume the doc directory exists
    # at make time, and the following install(DIRECTORY... must have doc exist
    # at cmake time.  Therefore, create the doc directory at CMake time.
    # (Linux experimentation indicates this is a no-op if the empty or
    # non-empty directory already exists.)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/doc/html)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/doc/html/images)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/doc/html/logos)

    CONFIGURE_FILE(DisCODe.Doxyfile.in ${CMAKE_BINARY_DIR}/DisCODe.Doxyfile @ONLY)

    ADD_CUSTOM_TARGET(
        doc
        COMMAND ${DOXYGEN_EXECUTABLE} DisCODe.Doxyfile
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/doc/search.js ${HTML_OUTPUT_DIR}/search/search.js
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/doc/search.css ${HTML_OUTPUT_DIR}/search/search.css
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/doc/images ${HTML_OUTPUT_DIR}/images
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/doc/logos ${HTML_OUTPUT_DIR}/logos
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS ${CMAKE_SOURCE_DIR}/doc/DisCODe.Doxyfile.in
    )
else(DOXYGEN_EXECUTABLE)
    message(STATUS "WARNING: Doxygen not found so documentation not generated.")
    ADD_CUSTOM_TARGET(
        doc
        COMMAND ${CMAKE_COMMAND} -E echo "+-------------------------------------------------"
        COMMAND ${CMAKE_COMMAND} -E echo "! Doxygen not found so documentation not generated"
        COMMAND ${CMAKE_COMMAND} -E echo "+-------------------------------------------------"
    )
endif(DOXYGEN_EXECUTABLE)
