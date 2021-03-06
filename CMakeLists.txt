
CMAKE_MINIMUM_REQUIRED( VERSION 2.8.4 )

project (RadInt)

#######################################
# Get the libs/inc for the dependencies
#######################################
find_package(PkgConfig REQUIRED)

pkg_search_module(GLEW REQUIRED glew)
pkg_search_module(GLFW REQUIRED glfw3)
pkg_search_module(FTGL REQUIRED ftgl)

#######################################
# Setting the compilation flags
#######################################

# cflags added by the package
SET(PROJECT_CFLAGS ${PROJECT_CFLAGS} "-Wall -std=c++11 -Iinclude -g")

# cflags added by the pkg-config dependencies contains ';' as separator. This is a fix.
string(REPLACE ";" " " GLEW_CFLAGS "${GLEW_CFLAGS}")
string(REPLACE ";" " " GLFW_CFLAGS "${GLFW_CFLAGS}")
string(REPLACE ";" " " FTGL_CFLAGS "${FTGL_CFLAGS}")

# ldflasgs added by the pkg-config dependencies contains ';' as separator. This is a fix.
string(REPLACE ";" " " GLEW_LDFLAGS "${GLEW_LDFLAGS}")
string(REPLACE ";" " " GLFW_LDFLAGS "${GLFW_LDFLAGS}")
string(REPLACE ";" " " VORBIS_LDFLAGS "${VORBIS_LDFLAGS}")

# Gathering of all flags
# (e.g. for compiling examples)
SET(PROJECT_ALL_CFLAGS  "${PROJECT_CFLAGS}" )

# Make sure the compiler can find include files from our library.
include_directories (${CMAKE_SOURCE_DIR}/src
  ${OPENGL_INCLUDE_DIR}
  ${FTGL_INCLUDE_DIRS}
  ${GLFW_INCLUDE_DIRS} )

###################################
#  Subdirectories
###################################
add_subdirectory(src)
add_subdirectory(test)

###################################
#  Make sure data and shaders available for build
###################################
## execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/data
##                                                            ${CMAKE_BINARY_DIR}/data)
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/src/shaders
                                                           ${CMAKE_BINARY_DIR}/src/shaders)
