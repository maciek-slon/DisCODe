# =======================================================================
# Additional macro definitions
# =======================================================================

# Ensures that we do an out of source build

MACRO(MACRO_ENSURE_OUT_OF_SOURCE_BUILD MSG)

    STRING(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" insource)
    GET_FILENAME_COMPONENT(PARENTDIR ${CMAKE_SOURCE_DIR} PATH)
    STRING(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${PARENTDIR}" insourcesubdir)
    
    IF(insource OR insourcesubdir)
        MESSAGE(FATAL_ERROR "${MSG}")
    ENDIF(insource OR insourcesubdir)
    
ENDMACRO(MACRO_ENSURE_OUT_OF_SOURCE_BUILD)


#
# Add variable controlling whether to build given corresponding component or not
# and - if set to TRUE - include component's subdirectory
#
# Optionally you can give additional argument to specify platform on which
# component can be built (use WIN32 for Windows-only and !WIN32 for *nix-only)
#
MACRO(ADD_COMPONENT COMP_NAME)
	if ( ${ARGC} LESS 2 )
		set( ARGV1 ${CMAKE_SYSTEM_NAME} )
	endif (${ARGC} LESS 2 )

	if ( ${CMAKE_SYSTEM_NAME} MATCHES ${ARGV1} )
		set(COMPONENT_${COMP_NAME} ON CACHE BOOL "Build ${COMP_NAME} component")
		if (COMPONENT_${COMP_NAME})
			MESSAGE(STATUS "\t${COMP_NAME}")
			ADD_SUBDIRECTORY(${COMP_NAME})
		endif(COMPONENT_${COMP_NAME})
	endif( ${CMAKE_SYSTEM_NAME} MATCHES ${ARGV1} )
ENDMACRO(ADD_COMPONENT)

#
# Set component's install directory and group
#
MACRO(INSTALL_COMPONENT COMP_NAME)
	SET_TARGET_PROPERTIES(${COMP_NAME} PROPERTIES
		BUILD_WITH_INSTALL_RPATH ON
		INSTALL_RPATH_USE_LINK_PATH 1
		INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib
	)

	install(
	TARGETS ${COMP_NAME}
		RUNTIME DESTINATION bin COMPONENT components
		LIBRARY DESTINATION lib COMPONENT components
		ARCHIVE DESTINATION lib
	)
ENDMACRO(INSTALL_COMPONENT)
