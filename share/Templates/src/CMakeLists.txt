SET(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(.)

# ##############################################################################
# Find required libraries
# ##############################################################################

# Find Boost, at least ver. 1.41
FIND_PACKAGE(Boost 1.41.0 REQUIRED COMPONENTS system thread filesystem date_time)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

# Set variable with list of all libraries common for this DCL
SET(DCL_LIBRARIES "${DisCODe_LIBRARIES} ${Boost_LIBRARIES}")

# Find another necessary libraries
# # OpenCV library
# FIND_PACKAGE( OpenCV REQUIRED )
# SET(DCL_LIBRARIES ${DCL_LIBRARIES} ${OpenCV_LIBS})

# Find another DCLs our one depends on
# Use macro: DISCODE_FIND_DCL(<DCL_NAME>)

# Check, whether all necessary libraries are linked
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,defs") 

# ##############################################################################
# Add source directories
# ##############################################################################

# CvBlobs components
ADD_SUBDIRECTORY(Components)

# CvBlobs types
ADD_SUBDIRECTORY(Types)

# Prepare config file to use from another DCLs
CONFIGURE_FILE(TEMPLATE_ARG_DCL_NAMEConfig.cmake.in ${CMAKE_INSTALL_PREFIX}/TEMPLATE_ARG_DCL_NAMEConfig.cmake @ONLY)
