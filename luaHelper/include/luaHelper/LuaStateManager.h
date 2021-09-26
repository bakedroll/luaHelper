#pragma once

#include <luaHelper/ILuaStateManager.h>

#include <osgHelper/ioc/Injector.h>

#include <memory>
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

  bool executeCodeString(const std::string& code) override;
  bool executeCodeFile(const std::string& filename) override;

  std::string getStackTrace() const override;
  bool        checkIsType(const luabridge::LuaRef& ref, int luaType) override;

  void safeExecute(const std::function<void()>& func) override;

protected:
  lua_State* getLuaState() const override;

private:
  std::map<std::string, LuaRefPtr> m_objectCache;

  lua_State* m_state;

  std::recursive_mutex m_luaLock;

  void logErrorsFromStack() const;
  void logLuaError(const std::string& message) const;

  enum class ExecuteMode
  {
    File,
    String
  };

  bool executeCode(const std::string& fileOrString, ExecuteMode mode);

};

}  // namespace luaHelper