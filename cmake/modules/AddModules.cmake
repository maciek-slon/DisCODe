# =======================================================================
# Additional macro definitions
# =======================================================================

#
# Add variable controlling whether to build given corresponding component or not
# and - if set to TRUE - include component's subdirectory
#
MACRO(ADD_PROCESSOR PROC_NAME)
    set(PROCESSORS_${PROC_NAME} ON CACHE BOOL "Build ${PROC_NAME} processor")
    if (PROCESSORS_${PROC_NAME})
        MESSAGE(STATUS "\t${PROC_NAME}")
        ADD_SUBDIRECTORY(${PROC_NAME})
    endif(PROCESSORS_${PROC_NAME})
ENDMACRO(ADD_PROCESSOR)

MACRO(ADD_SOURCE SRC_NAME)
    set(SOURCES_${SRC_NAME} ON CACHE BOOL "Build ${SRC_NAME} source")
    if (SOURCES_${SRC_NAME})
        MESSAGE(STATUS "\t${SRC_NAME}")
        ADD_SUBDIRECTORY(${SRC_NAME})
    endif(SOURCES_${SRC_NAME})
ENDMACRO(ADD_SOURCE)

#
# Set component's install directory and group
#
MACRO(INSTALL_PROCESSOR PROC_NAME)
    install(
        TARGETS ${PROC_NAME}
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION bin
        COMPONENT processors
    )
ENDMACRO(INSTALL_PROCESSOR)

MACRO(INSTALL_SOURCE SRC_NAME)
    install(
        TARGETS ${SRC_NAME}
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION bin
        COMPONENT sources
    )
ENDMACRO(INSTALL_SOURCE)
