#include "Lua.hpp"
#include "LuaManager.hpp"

namespace Cresta
{
	lua_State* LuaManager::L = nullptr;

	LuaManager::LuaManager()
	{
		L = luaL_newstate();  // Create a new Lua state
		luaL_openlibs(L);     // Load standard Lua libraries
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



	void LuaManager::CreateNewEnvironmentTable()
	{
		
	}

	void LuaManager::LoadFile(const std::string& path)
	{

	}

	void LuaManager::SetEnvironmentTable()
	{

	}

	void print_stack(lua_State* L) {
		int top = lua_gettop(L);  // Get the number of elements on the stack
		printf("Stack (top -> bottom):\n");
		for (int i = top; i > 0; i--) {
			int t = lua_type(L, i);
			switch (t) {
			case LUA_TSTRING:
				printf("%d: '%s'\n", i, lua_tostring(L, i));
				break;
			case LUA_TBOOLEAN:
				printf("%d: %s\n", i, lua_toboolean(L, i) ? "true" : "false");
				break;
			case LUA_TNUMBER:
				printf("%d: %g\n", i, lua_tonumber(L, i));
				break;
			case LUA_TFUNCTION:
				printf("%d: <function>\n", i);
				break;
			case LUA_TTABLE:
				printf("%d: <table>\n", i);
				break;
			case LUA_TLIGHTUSERDATA:
			case LUA_TUSERDATA:
				printf("%d: <userdata>\n", i);
				break;
			case LUA_TTHREAD:
				printf("%d: <thread>\n", i);
				break;
			case LUA_TNIL:
				printf("%d: nil\n", i);
				break;
			default:
				printf("%d: <unknown>\n", i);
				break;
			}
		}
		printf("\n");
	}

	void print_table(lua_State* L, int index) {
		if (!lua_istable(L, index)) {
			std::cout << "Value at index " << index << " is not a table.\n";
			return;
		}

		// Adjust index in case it's a negative stack index
		if (index < 0) index = lua_gettop(L) + index + 1;

		lua_pushnil(L);  // First key for lua_next

		std::cout << "{\n";

		while (lua_next(L, index) != 0) {
			// Key is at -2, value is at -1

			// Print key
			std::cout << "  [";
			if (lua_type(L, -2) == LUA_TSTRING)
				std::cout << lua_tostring(L, -2);
			else if (lua_type(L, -2) == LUA_TNUMBER)
				std::cout << lua_tonumber(L, -2);
			else
				std::cout << lua_typename(L, lua_type(L, -2));
			std::cout << "] = ";

			// Print value
			switch (lua_type(L, -1)) {
			case LUA_TSTRING:
				std::cout << "\"" << lua_tostring(L, -1) << "\"";
				break;
			case LUA_TNUMBER:
				std::cout << lua_tonumber(L, -1);
				break;
			case LUA_TBOOLEAN:
				std::cout << (lua_toboolean(L, -1) ? "true" : "false");
				break;
			case LUA_TTABLE:
				std::cout << "table";
				break;
			case LUA_TFUNCTION:
				std::cout << "function";
				break;
			case LUA_TNIL:
				std::cout << "nil";
				break;
			default:
				std::cout << lua_typename(L, lua_type(L, -1));
				break;
			}
			std::cout << ",\n";

			lua_pop(L, 1); // Remove value, keep key for next iteration
		}

		std::cout << "}\n";
	}

	void PrepareScriptComponent_(const std::string& path)
	{
		lua_State* L = luaL_newstate();

		if (luaL_loadfile(L, path.c_str()) != LUA_OK) {
			std::cerr << "Load Error: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
			return;
		}

		// Stack: [loaded_chunk]

		// Check if _ENV is upvalue #1
		const char* upvalueName = lua_getupvalue(L, -1, 1);
		if (!upvalueName || std::strcmp(upvalueName, "_ENV") != 0) {
			std::cerr << "No _ENV upvalue!" << std::endl;
			lua_pop(L, 1); // Pop the chunk
			return;
		}
		lua_pop(L, 1); // Pop the upvalue (leave chunk on stack)

		// Create environment table
		lua_newtable(L); // Stack: [loaded_chunk, env]

		// Set metatable { __index = _G }
		lua_newtable(L);
		lua_pushstring(L, "__index");
		lua_pushglobaltable(L);
		lua_settable(L, -3);
		lua_setmetatable(L, -2); // Stack: [loaded_chunk, env]

		// Set environment table as _ENV upvalue
		if (!lua_setupvalue(L, -2, 1)) { // -2 is the chunk
			std::cerr << "Failed to set _ENV upvalue!" << std::endl;
			lua_pop(L, 2); // Pop chunk + env
			return;
		}

		// Call the chunk
		if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
			std::cerr << "Runtime Error: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1); // Pop error message
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
	void LuaManager::PrepareScriptComponent(const std::string& script)
	{
		PrepareScriptComponent_("C:/dev/CrestaProjectFolder/test.lua");
	}
}