
CMAKE_MINIMUM_REQUIRED( VERSION 2.8.4 )

project (RadInt)

#######################################
# Get the libs/inc for the dependencies
#######################################

#######################################
# Setting the compilation flags
#######################################

# cflags added by the package
SET(PROJECT_CFLAGS ${PROJECT_CFLAGS} "-Wall -std=c++11 -Iinclude")

# Gathering of all flags
# (e.g. for compiling examples)
SET(PROJECT_ALL_CFLAGS  "${PROJECT_CFLAGS}" )

# Make sure the compiler can find include files from our library.
include_directories (${CMAKE_SOURCE_DIR}/src)

###################################
#  Subdirectories
###################################
add_subdirectory(src)
add_subdirectory(test)

