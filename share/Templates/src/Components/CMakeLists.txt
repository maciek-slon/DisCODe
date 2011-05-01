# Find Boost, at least ver. 1.41
FIND_PACKAGE(Boost 1.41.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# If it's necessary, add another libraries here.

# For OpenCV, uncomment following line
# FIND_PACKAGE( OpenCV REQUIRED )

# Add all components here using ADD_COMPONENT(<COMPONENT_DIRECTORY>)