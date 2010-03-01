# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(ExampleProcessor_SOURCES ExampleProcessor.cpp)

# Create an executable file called helloworld from sources:
add_library(ExampleProcessor SHARED ${ExampleProcessor_SOURCES})
