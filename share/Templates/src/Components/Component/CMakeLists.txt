# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create a variable containing all .cpp files:
FILE(GLOB files *.cpp)

# Create an executable file from sources:
ADD_LIBRARY(TemplateComponent SHARED ${files})

# Link external libraries
TARGET_LINK_LIBRARIES(TemplateComponent ${DCL_LIBRARIES})

# Add another libraries if necessary, for example:
# TARGET_LINK_LIBRARIES(Example ${DCL_LIBRARIES} ${Custom_LIBS} )

INSTALL_COMPONENT(TemplateComponent)
