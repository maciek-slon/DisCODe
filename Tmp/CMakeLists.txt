# Include the directory itself as a path to include directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Create a variable containing all .cpp files:
SET(ptree_files ptree.cpp)

# Create an executable file from sources:
ADD_EXECUTABLE(ptree ${ptree_files})
