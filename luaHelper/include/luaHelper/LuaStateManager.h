#pragma once

#include <luaHelper/ILuaStateManager.h>

#include <osgHelper/ioc/Injector.h>

#include <mutex>

extern "C"
{
#include <lua.h>
}

#include <LuaBridge/LuaBridge.h>

namespace luaHelper
{

class LuaStateManager : public ILuaStateManager
{
public:
  explicit LuaStateManager(osgHelper::ioc::Injector& injector);
  ~LuaStateManager() override;

  luabridge::LuaRef getGlobal(const char* name) const override;
  luabridge::LuaRef getObject(const char* name) override;
  luabridge::LuaRef newTable() const override;

  void setGlobal(const char* name, const luabridge::LuaRef& ref) override;

  void setCustomFileLoader(const std::function<int(lua_State*, const char*)>& loaderFunc) override;
  void setCustomPackageLoader(const std::function<int(lua_State*)>& loaderFunc) override;

  bool executeCodeString(const std::string& code) override;
  bool executeCodeFile(const std::string& filename) override;

  std::string getStackTrace() const override;
  bool checkIsType(const luabridge::LuaRef& ref, int luaType) override;

  void safeExecute(const std::function<void()>& func) override;

protected:
  lua_State* getLuaState() const override;

private:
  std::map<std::string, LuaRefPtr> m_objectCache;
  std::function<int(lua_State*, const char*)> m_customLoaderFunc;
  std::function<int(lua_State*)> m_customPackageLoader;

  lua_State* m_state;

  std::recursive_mutex m_luaLock;

  void logErrorsFromStack() const;
  void logLuaError(const std::string& message) const;

  enum class ExecuteMode
  {
    File,
    String,
    CustomFileLoader
  };

  bool executeCode(const std::string& fileOrString, ExecuteMode mode);

};

}