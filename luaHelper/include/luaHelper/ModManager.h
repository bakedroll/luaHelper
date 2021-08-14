#pragma once

#include <luaHelper/IModManager.h>
#include <luaHelper/ILuaStateManager.h>

#include <osgHelper/ioc/Injector.h>

namespace luaHelper
{
  class ModManager : public IModManager
  {
  public:
    explicit ModManager(osgHelper::ioc::Injector& injector);
    ~ModManager() override;

    void loadModFromDirectory(const std::string& path) override;
    void scanDirectoryForMods(const std::string& path) override;

  private:
    ILuaStateManager::Ptr m_lua;
    std::string           m_pathString;

    void clearLoadedLuaPackages();
    void addPathToLuaPackage(const std::string& path);

  };

}
