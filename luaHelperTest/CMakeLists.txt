begin_project(luaHelperTest EXECUTABLE OPTIONAL)

require_library(GTest)
require_library(OpenSceneGraph MODULES osg osgViewer osgUtil osgGA osgDB osgText OpenThreads)
require_library(osgPPU)

require_library(Lua)
require_library(LuaBridge)

require_project(luaHelper)
require_project(osgHelper PATH osgHelper)

add_source_directory(core)
add_source_directory(tests)