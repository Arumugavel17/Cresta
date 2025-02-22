#pragma once
#include "Crestaph.hpp"
#include "ECS/Components/Components.hpp"

namespace Cresta
{
	extern "C" 
	{
		#include "lua.h"
		#include "lualib.h"
		#include "lauxlib.h"
	}

	static class LuaManager
	{
	public:
		LuaManager();
		~LuaManager();

		static void CreateNewEnvironmentTable();
		static void LoadFile(const std::string& path);
		static void SetEnvironmentTable();
		static void RunScript(const std::string& script);
		static void PrepareScriptComponent(const std::string& script);

		static int callLuaFunction(const std::string& function, int a, int b);
	private:
		static lua_State* L;
	};

	class Script : public Component
	{
	public:
		Script(const std::string& ScriptPath)
		{
			LuaManager::PrepareScriptComponent(ScriptPath);
		}

		std::string ToString() override
		{
			return "Script Component";
		}
	};
}