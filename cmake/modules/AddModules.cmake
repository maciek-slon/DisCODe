# =======================================================================
# Additional macro definitions
# =======================================================================

#
# Add variable controlling whether to build given corresponding component or not
# and - if set to TRUE - include component's subdirectory
#
# Optionally you can give additional argument to specify platform on which 
# component can be built (use WIN32 for Windows-only and !WIN32 for *nix-only)
#
MACRO(ADD_PROCESSOR PROC_NAME)
	if ( ${ARGC} LESS 2 )
		set( ARGV1 ${CMAKE_SYSTEM_NAME} )
	endif (${ARGC} LESS 2 )
	
	if ( ${CMAKE_SYSTEM_NAME} MATCHES ${ARGV1} )
		set(PROCESSORS_${PROC_NAME} ON CACHE BOOL "Build ${PROC_NAME} processor")
		if (PROCESSORS_${PROC_NAME})
			MESSAGE(STATUS "\t${PROC_NAME}")
			ADD_SUBDIRECTORY(${PROC_NAME})
		endif(PROCESSORS_${PROC_NAME})
	endif( ${CMAKE_SYSTEM_NAME} MATCHES ${ARGV1} )
ENDMACRO(ADD_PROCESSOR)

MACRO(ADD_SOURCE SRC_NAME)
	if ( ${ARGC} LESS 2 )
		set( ARGV1 ${CMAKE_SYSTEM_NAME} )
	endif (${ARGC} LESS 2 )
	
	if ( ${CMAKE_SYSTEM_NAME} MATCHES ${ARGV1} )
		set(SOURCES_${SRC_NAME} ON CACHE BOOL "Build ${SRC_NAME} source")
		if (SOURCES_${SRC_NAME})
			MESSAGE(STATUS "\t${SRC_NAME}")
			ADD_SUBDIRECTORY(${SRC_NAME})
		endif(SOURCES_${SRC_NAME})
	endif( ${CMAKE_SYSTEM_NAME} MATCHES ${ARGV1} )
ENDMACRO(ADD_SOURCE)

MACRO(ADD_SINK SINK_NAME)
	if ( ${ARGC} LESS 2 )
		set( ARGV1 ${CMAKE_SYSTEM_NAME} )
	endif (${ARGC} LESS 2 )

	if ( ${CMAKE_SYSTEM_NAME} MATCHES ${ARGV1} )
		set(SINKS_${SINK_NAME} ON CACHE BOOL "Build ${SINK_NAME} sink")
		if (SINKS_${SINK_NAME})
			MESSAGE(STATUS "\t${SINK_NAME}")
			ADD_SUBDIRECTORY(${SINK_NAME})
		endif(SINKS_${SINK_NAME})
	endif( ${CMAKE_SYSTEM_NAME} MATCHES ${ARGV1} )
ENDMACRO(ADD_SINK)

MACRO(ADD_PROXY PROXY_NAME)
	if ( ${ARGC} LESS 2 )
		set( ARGV1 ${CMAKE_SYSTEM_NAME} )
	endif (${ARGC} LESS 2 )

	if ( ${CMAKE_SYSTEM_NAME} MATCHES ${ARGV1} )
		set(PROXIES_${PROXY_NAME} ON CACHE BOOL "Build ${POXY_NAME} proxy")
		if (PROXIES_${PROXY_NAME})
			MESSAGE(STATUS "\t${PROXY_NAME}")
			ADD_SUBDIRECTORY(${PROXY_NAME})
		endif(PROXIES_${PROXY_NAME})
	endif( ${CMAKE_SYSTEM_NAME} MATCHES ${ARGV1} )
ENDMACRO(ADD_PROXY)


#
# Set component's install directory and group
#
MACRO(INSTALL_PROCESSOR PROC_NAME)
    SET_TARGET_PROPERTIES(${PROC_NAME}
        PROPERTIES BUILD_WITH_INSTALL_RPATH OFF
        INSTALL_RPATH .
    )

    install(
        TARGETS ${PROC_NAME}
		RUNTIME DESTINATION bin COMPONENT processors
        LIBRARY DESTINATION bin COMPONENT processors
        ARCHIVE DESTINATION bin 
    )
ENDMACRO(INSTALL_PROCESSOR)

MACRO(INSTALL_SOURCE SRC_NAME)
    SET_TARGET_PROPERTIES(${SRC_NAME}
        PROPERTIES BUILD_WITH_INSTALL_RPATH OFF
        INSTALL_RPATH .
    )
    
    install(
        TARGETS ${SRC_NAME}
		RUNTIME DESTINATION bin COMPONENT sources
        LIBRARY DESTINATION bin COMPONENT sources
        ARCHIVE DESTINATION bin 
    )
ENDMACRO(INSTALL_SOURCE)

MACRO(INSTALL_SINK SINK_NAME)
    SET_TARGET_PROPERTIES(${SINK_NAME}
        PROPERTIES BUILD_WITH_INSTALL_RPATH OFF
        INSTALL_RPATH .
    )

    install(
        TARGETS ${SINK_NAME}
		RUNTIME DESTINATION bin COMPONENT sinks
        LIBRARY DESTINATION bin COMPONENT sinks
        ARCHIVE DESTINATION bin 
    )
ENDMACRO(INSTALL_SINK)

MACRO(INSTALL_PROXY PROXY_NAME)
    install(
        TARGETS ${PROXY_NAME}
		RUNTIME DESTINATION bin COMPONENT proxies
        LIBRARY DESTINATION bin COMPONENT proxies
        ARCHIVE DESTINATION bin 
    )
ENDMACRO(INSTALL_PROXY)
