#pragma once
#include "Crestaph.hpp"
#include "ECS/Components/Components.hpp"
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace Cresta
{
	class LuaManager
	{
	public:
		static void Init();
		static void CreateNewEnvironmentTable();
		static void LoadFile(const std::string& path);
		static void SetEnvironmentTable();
		static void RunScript(const std::string& script);
		static void RunString(const std::string& script);
		static void PrepareScriptComponent(const std::string& script);

		static int callLuaFunction(const std::string& function, int a, int b);
	private:
		static sol::state sm_lua;
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