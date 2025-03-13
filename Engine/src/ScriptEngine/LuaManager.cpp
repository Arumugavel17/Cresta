#include "LuaManager.hpp"

namespace Cresta
{
	sol::state LuaManager::sm_lua;

	void LuaManager::Init()
	{
		sm_lua.open_libraries(sol::lib::base);
	}

	void LuaManager::RunString(const std::string& script)
	{
		sm_lua.script(script);
	}

	void LuaManager::RunScript(const std::string& script)
	{
		sm_lua.script_file("variables.lua");
		bool isfullscreen = sm_lua["config"]["fullscreen"]; 
		sol::table config = sm_lua["config"];
		config;
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

	int LuaManager::callLuaFunction(const std::string& function, int a, int b)
	{
		return 0;
	}

	void LuaManager::PrepareScriptComponent(const std::string& script)
	{
	}
}