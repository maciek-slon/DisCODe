
# Add subdirectories to the build - CMake will run CMakeFiles from them
add_subdirectory(Tasks)
add_subdirectory(SDK)

install(FILES DisCODeMacros.cmake DESTINATION ${CMAKE_INSTALL_PREFIX}/share/DisCODe/)
