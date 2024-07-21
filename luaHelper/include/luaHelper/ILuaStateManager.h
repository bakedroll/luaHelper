#pragma once

#include <luaHelper/LuaBridgeDefinition.h>

#include <utilsLib/Utils.h>

#include <osg/Referenced>
#include <osg/ref_ptr>

extern "C"
{
#include <lua.h>
}

#include <LuaBridge/LuaBridge.h>

#include <functional>
#include <string>

#define MAKE_LUAREF_PTR(luaRef) std::make_shared<luabridge::LuaRef>(luaRef)

namespace luaHelper
{

using LuaRefPtr = std::shared_ptr<luabridge::LuaRef>;

class ILuaStateManager : public osg::Referenced
{
public:
  using Ptr = osg::ref_ptr<ILuaStateManager>;

  enum class RethrowErrors
  {
    YES,
    NO
  };

  ILuaStateManager() = default;
  virtual ~ILuaStateManager() = default;

  virtual luabridge::LuaRef getGlobal(const char* name) const = 0;
  virtual luabridge::LuaRef getObject(const char* name) = 0;
  virtual luabridge::LuaRef newTable() const = 0;

  virtual void setGlobal(const char* name, const luabridge::LuaRef& ref) = 0;

  virtual void setCustomFileLoader(const std::function<int(lua_State*, const char*)>& loaderFunc) = 0;
  virtual void setCustomPackageLoader(const std::function<int(lua_State*)>& loaderFunc) = 0;

  virtual bool executeCodeString(const std::string& code, RethrowErrors rethrowErrors = RethrowErrors::NO) = 0;
  virtual bool executeCodeFile(const std::string& filename, RethrowErrors rethrowErrors = RethrowErrors::NO) = 0;

  virtual std::string getStackTrace() const = 0;
  virtual bool checkIsType(const luabridge::LuaRef& ref, int luaType) = 0;

  virtual void safeExecute(const std::function<void()>& func, RethrowErrors rethrowErrors = RethrowErrors::NO) = 0;

  template <typename LuaObject>
  std::shared_ptr<LuaObject> createTableMappedObject(const luabridge::LuaRef& table)
  {
    assert_return(checkIsType(table, LUA_TTABLE), nullptr);
    return std::make_shared<LuaObject>(table, getLuaState());
  }

  template <typename DefinitionType,
    typename = typename std::enable_if<std::is_base_of<luaHelper::LuaBridgeDefinition, DefinitionType>::value>::type,
    typename... Args>
    void registerDefinition(Args&&... args)
  {
    DefinitionType(std::forward<Args>(args)...).registerDefinition(getLuaState());
  }

protected:
  virtual lua_State* getLuaState() const = 0;

};

}