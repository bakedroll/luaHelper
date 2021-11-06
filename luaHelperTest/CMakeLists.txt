begin_project(luaHelperTest EXECUTABLE OPTIONAL)

require_library(GTest)
require_library(OpenSceneGraph MODULES osg OpenThreads)

require_library(Lua)
require_library(LuaBridge)

require_project(luaHelper)
require_project(osgHelper PATH osgHelper)
require_project(utilsLib PATH utilsLib)

add_source_directory(core)
add_source_directory(tests)

add_project_definitions(-DQT_NO_VERSION_TAGGING)