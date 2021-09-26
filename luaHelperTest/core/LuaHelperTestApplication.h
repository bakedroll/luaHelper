#pragma once

#include <osgHelper/ioc/PointerTypeDefinition.h>
#include <osgHelper/ioc/InjectionContainer.h>
#include <osgHelper/ioc/Injector.h>

#include <osgHelper/GameApplication.h>

namespace luaHelperTest
{

class LuaHelperTestApplication : public osgHelper::GameApplication
{
public:
  int runTests(int argc, char** argv);

  static osgHelper::ioc::Injector& getInjector();

protected:
  void registerComponents(osgHelper::ioc::InjectionContainer& container);

  static std::unique_ptr<osgHelper::ioc::Injector> m_staticInjector;
  osgHelper::ioc::InjectionContainer               m_container;
};

}
