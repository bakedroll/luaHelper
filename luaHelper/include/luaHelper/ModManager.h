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

    void loadModFromDirectory(const std::string& path, ILuaStateManager::RethrowErrors rethrowErrors) override;
    void scanDirectoryForMods(const std::string& path, ILuaStateManager::RethrowErrors rethrowErrors) override;

    void setCustomFilesystemExists(const std::function<bool(const std::string&)>& existsFunc) override;

  private:
    ILuaStateManager::Ptr m_lua;
    std::string m_pathString;

    std::function<bool(const std::string&)> m_filesystemExistsFunc;

    void clearLoadedLuaPackages();
    void addPathToLuaPackage(const std::string& path);

  };

}
