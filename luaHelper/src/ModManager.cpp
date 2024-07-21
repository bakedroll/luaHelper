#include <luaHelper/ModManager.h>

#include <filesystem>

namespace luaHelper
{

static bool filesystemExists(const std::string& path)
{
  const std::filesystem::path p(path);
  return std::filesystem::exists(p);
}

ModManager::ModManager(osgHelper::ioc::Injector& injector) :
  IModManager(),
  m_lua(injector.inject<ILuaStateManager>()),
  m_filesystemExistsFunc(filesystemExists)
{
}

ModManager::~ModManager() = default;

void ModManager::loadModFromDirectory(const std::string& path, ILuaStateManager::RethrowErrors rethrowErrors)
{
  UTILS_LOG_INFO(std::string("Loading scripts from directory ") + path);
  if (!m_filesystemExistsFunc(path))
  {
    UTILS_LOG_WARN("Directory '" + path + "' does not exist.");
    return;
  }

  std::filesystem::path dataLuaFilepath(path);
  std::filesystem::path controlLuaFilepath(path);

  const std::string dataLuaFilename("data.lua");
  const std::string controlLuaFilename("control.lua");

  dataLuaFilepath /= dataLuaFilename;
  controlLuaFilepath /= controlLuaFilename;

  const auto dataScriptExists = m_filesystemExistsFunc(dataLuaFilepath.string());
  const auto controlScriptExists = m_filesystemExistsFunc(controlLuaFilepath.string());

  if (!dataScriptExists && !controlScriptExists)
  {
    UTILS_LOG_WARN(std::string("Neither ") + dataLuaFilename + " nor " + controlLuaFilename +
                  " exist in the directory " + path + ". No scripts were loaded");
    return;
  }

  addPathToLuaPackage(path);
  clearLoadedLuaPackages();

  if (dataScriptExists)
  {
    m_lua->executeCodeFile(dataLuaFilepath.string(), rethrowErrors);
  }

  if (controlScriptExists)
  {
    m_lua->executeCodeFile(controlLuaFilepath.string(), rethrowErrors);
  }
}

void ModManager::scanDirectoryForMods(const std::string& path, ILuaStateManager::RethrowErrors rethrowErrors)
{
  if (!m_filesystemExistsFunc(path))
  {
    UTILS_LOG_WARN("Directory '" + path + "' does not exist.");
    return;
  }

  for (auto& entry : std::filesystem::directory_iterator(path))
  {
    if (entry.is_directory())
    {
      loadModFromDirectory(entry.path().string(), rethrowErrors);
    }
  }
}

void ModManager::setCustomFilesystemExists(const std::function<bool(const std::string&)>& existsFunc)
{
  m_filesystemExistsFunc = existsFunc;
}

void ModManager::clearLoadedLuaPackages()
{
  m_lua->safeExecute([this]()
  {
    const luabridge::LuaRef loadedRef = m_lua->getObject("package.loaded");

    if (!loadedRef.isTable())
    {
      UTILS_LOG_FATAL("Expected package.loaded to be a table");
      return;
    }

    for (luabridge::Iterator it(loadedRef); !it.isNil(); ++it)
    {
      luabridge::LuaRef keyRef   = it.key();
      luabridge::LuaRef valueRef = it.value();

      if (valueRef.isBool() && keyRef.isString())
      {
        loadedRef[keyRef] = false;
      }
    }
  });
}

void ModManager::addPathToLuaPackage(const std::string& path)
{
  m_lua->safeExecute([this, path]()
  {
    const luabridge::LuaRef packageRef = m_lua->getGlobal("package");

    if (m_pathString.empty())
    {
      const luabridge::LuaRef packagePath = packageRef["path"];

      if (!packagePath.isString())
      {
        UTILS_LOG_FATAL("Expected package.path to be a string");
        return;
      }

      m_pathString = packagePath.tostring();
    }

    auto packagePathStr = m_pathString;

    std::filesystem::path fpath(path);
    fpath /= "?.lua";

    packagePathStr.append(";").append(fpath.string());

    packageRef["path"] = packagePathStr;
  });
}

}
