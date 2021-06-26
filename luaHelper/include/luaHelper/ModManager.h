#pragma once

#include <luaHelper/LuaStateManager.h>

#include <osg/ref_ptr>
#include <osg/Referenced>

#include <osgHelper/ioc/Injector.h>

#include <string>

namespace luaHelper
{

  class ModManager : public osg::Referenced
  {
  public:
    using Ptr = osg::ref_ptr<ModManager>;

    explicit ModManager(osgHelper::ioc::Injector& injector);
    virtual ~ModManager();

    void loadModFromDirectory(const std::string& path);
    void scanDirectoryForMods(const std::string& path);

  private:
    LuaStateManager::Ptr m_lua;

    void addPathToLuaPackage(const std::string& path);

  };

}
