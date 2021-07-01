#include "core/LuaHelperTestApplication.h"

int main(int argc, char** argv)
{
  luaHelperTest::LuaHelperTestApplication app;
  return app.runTests(argc, argv);
}