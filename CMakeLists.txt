cmake_minimum_required(VERSION 3.4)

set(CMAKE_SCRIPTS_DIRECTORY "" CACHE STRING "The CMake scripts directory")
if(EXISTS ${CMAKE_SCRIPTS_DIRECTORY}/scripts.cmake)
  include(${CMAKE_SCRIPTS_DIRECTORY}/scripts.cmake)
else()
  message(FATAL_ERROR "Please specify CMAKE_SCRIPTS_DIRECTORY")
  return()
endif()

project(luaHelper)

add_subdirectory(luaHelper)
add_subdirectory(luaHelperTest)

make_projects()

if (MSVC)
  osg_copy_additional_binaries(zlib)
endif()
