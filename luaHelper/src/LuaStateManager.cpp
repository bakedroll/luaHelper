#include <luaHelper/LuaStateManager.h>
#include <luaHelper/LuaInvalidDataException.h>

#include <osgHelper/ResourceManager.h>
#include <osgHelper/Macros.h>

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
  struct LuaStateManager::Impl
  {
    Impl(osgHelper::ioc::Injector& injector)
      : resourceManager(injector.inject<osgHelper::ResourceManager>())
    {}

    osg::ref_ptr<osgHelper::ResourceManager> resourceManager;
    std::map<std::string, LuaRefPtr>         objectCache;

  };

  LuaStateManager::LuaStateManager(osgHelper::ioc::Injector& injector)
    : osg::Referenced()
    , m(new Impl(injector))
  {
    std::lock_guard<std::recursive_mutex> lock(m_luaLock);

    m_state = luaL_newstate();

    luaL_openlibs(m_state);
  }

  LuaStateManager::~LuaStateManager()
  {
    std::lock_guard<std::recursive_mutex> lock(m_luaLock);
    m->objectCache.clear();

    lua_close(m_state);
  }

  luabridge::LuaRef LuaStateManager::getGlobal(const char* name) const
  {
    return luabridge::getGlobal(m_state, name);
  }

  luabridge::LuaRef LuaStateManager::getObject(const char* name) const
  {
    const std::string namestr(name);
    if (m->objectCache.count(namestr) > 0)
    {
      return *m->objectCache[namestr].get();
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

    m->objectCache[namestr] = MAKE_LUAREF_PTR(current);
    return current;
  }

  luabridge::LuaRef LuaStateManager::newTable() const
  {
    return luabridge::newTable(m_state);
  }

  luabridge::LuaRef LuaStateManager::createGlobalTable(const std::string& name) const
  {
    luabridge::LuaRef table = newTable();
    luabridge::setGlobal(m_state, table, name.c_str());
    return table;
  }

  luabridge::LuaRef LuaStateManager::createTable(const std::string& name, const luabridge::LuaRef& parentTable)
  {
    luabridge::LuaRef table = newTable();
    parentTable[name] = table;
    return table;
  }

  luabridge::LuaRef LuaStateManager::copyTable(const luabridge::LuaRef& table) const
  {
    const auto copyFunc = getGlobal("helper.deepcopy");
    assert_return(copyFunc.isFunction(), luabridge::LuaRef(m_state));

    return copyFunc(table);
  }

  void LuaStateManager::setGlobal(const char* name, const luabridge::LuaRef& ref)
  {
    luabridge::setGlobal(m_state, ref, name);
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
    auto        lines = 0;

    luaL_traceback(m_state, m_state, nullptr, 0);
    while (lua_gettop(m_state) > 0)
    {
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
    OSGH_LOG_WARN(message);

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

    OSGH_LOG_FATAL(log);
  }

  bool LuaStateManager::executeCode(const std::string& fileOrString, ExecuteMode mode)
  {
    auto success = true;
    safeExecute([this, mode, &success, &fileOrString]()
    {
      if (((mode == ExecuteMode::File) && (luaL_dofile(m_state, fileOrString.c_str()) != LUA_OK)) ||
        ((mode == ExecuteMode::String) && (luaL_dostring(m_state, fileOrString.c_str()) != LUA_OK)))
      {
        logErrorsFromStack();
        success = false;
      }
    });

    return success;
  }
}
