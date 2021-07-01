#pragma once

#include <osgHelper/GameApplication.h>

namespace luaHelperTest
{

class LuaHelperTestApplication : public osgHelper::GameApplication
{
public:
  int runTests(int argc, char** argv);

  static osgHelper::ioc::Injector& getInjector();

protected:
  void registerComponents(osgHelper::ioc::InjectionContainer& container) override;

  static osgHelper::ioc::Injector* m_staticInjector;
};

}