#include <luaHelper/LuaStateManager.h>
#include <luaHelper/LuaInvalidDataException.h>

#include <osgHelper/IResourceManager.h>

#include <algorithm>
#include <string>
#include <sstream>

extern "C"
{
#include <lauxlib.h>
#include <lualib.h>
#include <lua.h>
}

namespace luaHelper
{

LuaStateManager::LuaStateManager(osgHelper::ioc::Injector& injector)
  : ILuaStateManager()
{
  std::lock_guard<std::recursive_mutex> lock(m_luaLock);

  m_state = luaL_newstate();

  luaL_requiref(m_state, "package", luaopen_package, 1);
  luaL_openlibs(m_state);
}

LuaStateManager::~LuaStateManager()
{
  std::lock_guard<std::recursive_mutex> lock(m_luaLock);
  m_objectCache.clear();

  lua_close(m_state);
}

luabridge::LuaRef LuaStateManager::getGlobal(const char* name) const
{
  return luabridge::getGlobal(m_state, name);
}

luabridge::LuaRef LuaStateManager::getObject(const char* name)
{
  const std::string namestr(name);
  if (m_objectCache.count(namestr) > 0)
  {
    return *m_objectCache.at(namestr).get();
  }

  std::vector<std::string> names;
  std::istringstream f(namestr);
  std::string element;

  while (getline(f, element, '.'))
  {
    names.push_back(element);
  }

  assert_return(!names.empty(), luabridge::LuaRef(m_state));

  std::string             currentName = names[0];
  const luabridge::LuaRef first       = luabridge::getGlobal(m_state, currentName.c_str());
  luabridge::LuaRef       current     = first;

  for (auto i = 1; i < static_cast<int>(names.size()); i++)
  {
    assert_return(current.isTable(), luabridge::LuaRef(m_state));

    currentName = names[i];
    const luabridge::LuaRef next = current[currentName.c_str()];
    current = next;
  }

  m_objectCache[namestr] = MAKE_LUAREF_PTR(current);
  return current;
}

luabridge::LuaRef LuaStateManager::newTable() const
{
  return luabridge::newTable(m_state);
}

void LuaStateManager::setGlobal(const char* name, const luabridge::LuaRef& ref)
{
  luabridge::setGlobal(m_state, ref, name);
}

void LuaStateManager::setCustomFileLoader(const std::function<int(lua_State*, const char*)>& loaderFunc)
{
  m_customLoaderFunc = loaderFunc;
}

void LuaStateManager::setCustomPackageLoader(const std::function<int(lua_State*)>& loaderFunc)
{
  std::lock_guard<std::recursive_mutex> lock(m_luaLock);

  m_customPackageLoader = loaderFunc;
  auto* funcPtr = m_customPackageLoader.target<int(*)(lua_State*)>();

  const auto packageRef = getGlobal("package");
  if (!packageRef.isTable())
  {
    UTILS_LOG_ERROR("Lua table 'package' not found.");
    return;
  }

  const auto searchersRef = packageRef["searchers"];
  if (!searchersRef.isTable())
  {
    UTILS_LOG_ERROR("Lua table 'package.searchers' not found.");
    return;
  }

  searchersRef.append(*funcPtr);
}

bool LuaStateManager::executeCodeString(const std::string& code)
{
  return executeCode(code, ExecuteMode::String);
}

bool LuaStateManager::executeCodeFile(const std::string& filename)
{
  return executeCode(filename, ExecuteMode::File);
}

std::string LuaStateManager::getStackTrace() const
{
  std::string result;
  auto lines = 0;

  luaL_traceback(m_state, m_state, nullptr, 0);
  while (lua_gettop(m_state) > 0)
  {
    if (!lua_isstring(m_state, -1))
    {
      lua_pop(m_state, 1);
      continue;
    }

    result.append(lua_tostring(m_state, -1));
    result.append("\n");

    lua_pop(m_state, 1);
    lines++;
  }

  return (lines > 1 ? result : "");
}

bool LuaStateManager::checkIsType(const luabridge::LuaRef& ref, int luaType)
{
  if (ref.type() == luaType)
  {
    return true;
  }

  std::string message = "Lua typecheck failed\n";
  message.append(getStackTrace());
  UTILS_LOG_WARN(message);

  return false;
}

void LuaStateManager::safeExecute(const std::function<void()>& func)
{
  try
  {
    std::lock_guard<std::recursive_mutex> locker(m_luaLock);
    func();
  }
  catch (luabridge::LuaException& e)
  {
    logLuaError(e.what());
  }
  catch (LuaInvalidDataException& e)
  {
    logLuaError(e.what());
  }
}

lua_State* LuaStateManager::getLuaState() const
{
  return m_state;
}

void LuaStateManager::logErrorsFromStack() const
{
  std::string msg = lua_tostring(m_state, -1);

  std::replace(msg.begin(), msg.end(), '\r', '\n');
  lua_pop(m_state, 1);

  logLuaError(msg);
}

void LuaStateManager::logLuaError(const std::string& message) const
{
  auto log = std::string("Lua Error: ") + message;

  const auto stacktrace = getStackTrace();
  if (!stacktrace.empty())
  {
    log.append("\n");
    log.append(stacktrace);
  }

  UTILS_LOG_FATAL(log);
}

bool LuaStateManager::executeCode(const std::string& fileOrString, ExecuteMode mode)
{
  auto success = true;
  safeExecute([this, mode, &success, &fileOrString]()
  {
    auto result = 0;
    if (mode == ExecuteMode::File)
    {
      if (m_customLoaderFunc)
      {
        result = m_customLoaderFunc(m_state, fileOrString.c_str());
      }
      else
      {
        result = luaL_dofile(m_state, fileOrString.c_str());
      }
    }
    else if (mode == ExecuteMode::String)
    {
      result = luaL_dostring(m_state, fileOrString.c_str());
    }

    if (result != LUA_OK)
    {
      logErrorsFromStack();
      success = false;
    }
  });

  return success;
}

}
