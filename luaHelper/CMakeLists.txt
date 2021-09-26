begin_project(luaHelper LIBRARY)

require_library(OpenSceneGraph MODULES osg)
require_library(Lua)
require_library(LuaBridge)

require_project(utilsLib PATH utilsLib)
require_project(osgHelper PATH osgHelper)

if (LUADOC_ENABLE)
  require_project(luadoc PATH luadoc)
endif()

add_source_directory(src)
add_source_directory(include)

add_include_directory(include)
