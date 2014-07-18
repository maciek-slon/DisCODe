get_filename_component(DCLNAME ${CMAKE_SOURCE_DIR} NAME_WE)
install(CODE "MESSAGE(\"Refreshing ${DCLNAME} cache...\")")
install(CODE "execute_process(COMMAND dclcache ${DCLNAME})")
