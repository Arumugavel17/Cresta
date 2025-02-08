#include "Lua.hpp"
#include "LuaManager.hpp"

namespace Cresta
{
	LuaManager::LuaManager()
	{
		L = luaL_newstate();  // Create a new Lua state
		luaL_openlibs(L);     // Load standard Lua libraries
		luaL_dostring(L, "function add(a, b) return a + b end");
	}

	LuaManager::~LuaManager()
	{
		lua_close(L);  // Close Lua state
	}

	void LuaManager::RunScript(const std::string& script)
	{
		if (luaL_dofile(L, script.c_str())) {
			std::cerr << "Error: " << lua_tostring(L, -1) << std::endl;
		}
	}

	int LuaManager::callLuaFunction(const std::string& function, int a, int b)
	{
		lua_getglobal(L, function.c_str());  // Get function by name
		lua_pushnumber(L, a);  // Push argument 1
		lua_pushnumber(L, b);  // Push argument 2

		if (lua_pcall(L, 2, 1, 0) != LUA_OK) {  // Call with 2 args, expecting 1 return value
			std::cerr << "Lua Error: " << lua_tostring(L, -1) << std::endl;
			return 0;
		}

		int result = lua_tonumber(L, -1);  // Get return value
		lua_pop(L, 1);  // Remove result from stack
		return result;
	}
}