#include <luaHelper/ModManager.h>

#include <filesystem>

namespace luaHelper
{

ModManager::ModManager(osgHelper::ioc::Injector& injector)
  : osg::Referenced()
  , m_lua(injector.inject<LuaStateManager>())
{
}

ModManager::~ModManager() = default;

void ModManager::loadModFromDirectory(const std::string& path)
{
  OSGH_LOG_INFO(std::string("Loading scripts from directory ") + path);

  const std::filesystem::path dir(path);
  std::filesystem::path       dataLuaFilepath(path);
  std::filesystem::path       controlLuaFilepath(path);

  const std::string dataLuaFilename("data.lua");
  const std::string controlLuaFilename("control.lua");

  dataLuaFilepath    /= dataLuaFilename;
  controlLuaFilepath /= controlLuaFilename;

  const auto dataScriptExists    = std::filesystem::exists(dataLuaFilepath);
  const auto controlScriptExists = std::filesystem::exists(controlLuaFilepath);

  if (!dataScriptExists && !controlScriptExists)
  {
    OSGH_LOG_WARN(std::string("Neither ") + dataLuaFilename + " nor " + controlLuaFilename +
                  " exist in the directory " + path + ". No scripts were loaded");
    return;
  }

  addPathToLuaPackage(std::filesystem::absolute(dir).string());

  if (dataScriptExists)
  {
    m_lua->executeCodeFile(dataLuaFilepath.string());
  }

  if (controlScriptExists)
  {
    m_lua->executeCodeFile(controlLuaFilepath.string());
  }
}

void ModManager::scanDirectoryForMods(const std::string& path)
{
  const std::filesystem::path dir(path);
  for (auto& entry : std::filesystem::directory_iterator(path))
  {
    if (entry.is_directory())
    {
      auto modPath = dir / entry.path();
      loadModFromDirectory(modPath.string());
    }
  }
}

void ModManager::addPathToLuaPackage(const std::string& path)
{
  m_lua->safeExecute([this, path]()
  {
    luabridge::LuaRef packageRef  = m_lua->getGlobal("package");
    luabridge::LuaRef packagePath = packageRef["path"];

    if (!packagePath.isString())
    {
      OSGH_LOG_FATAL("Expected package.path to be a string");
      return;
    }

    auto packagePathStr = packagePath.tostring();

    std::filesystem::path fpath(path);
    fpath /= "?.lua";

    packagePathStr.append(";").append(fpath.string());

    packageRef["path"] = packagePathStr;
  });
}

}
