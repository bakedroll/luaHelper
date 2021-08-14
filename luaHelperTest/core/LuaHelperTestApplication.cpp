#include "core/LuaHelperTestApplication.h"

#include <luaHelper/LuaStateManager.h>
#include <luaHelper/ModManager.h>

#include <gtest/gtest.h>

namespace luaHelperTest
{

int LuaHelperTestApplication::runTests(int argc, char** argv)
{
  setupIOC();

  m_staticInjector = &injector();

  testing::InitGoogleTest(&argc, argv);
  const auto result = RUN_ALL_TESTS();

  return result;
}

osgHelper::ioc::Injector& LuaHelperTestApplication::getInjector()
{
  return *m_staticInjector;
}

void LuaHelperTestApplication::registerComponents(osgHelper::ioc::InjectionContainer& container)
{
  registerEssentialComponents();

  container.registerSingletonInterfaceType<luaHelper::ILuaStateManager, luaHelper::LuaStateManager>();
  container.registerSingletonInterfaceType<luaHelper::IModManager, luaHelper::ModManager>();
}

osgHelper::ioc::Injector* LuaHelperTestApplication::m_staticInjector = nullptr;

}
