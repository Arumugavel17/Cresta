#pragma once
#include "Crestaph.hpp"
#include "Components.hpp"
#include "ScriptEngine/LuaManager.hpp"

namespace Cresta
{
	class ScriptComponent : public Component
	{
	public:
		ScriptComponent(std::filesystem::path ScriptPath) : m_ScriptPath(ScriptPath)
		{
			m_lua.open_libraries(sol::lib::base);

			m_lua.set_function("RegisterEntity", [this](sol::table entityTable) {
				this->m_ComponentTable = entityTable;
				});

			m_lua.script_file(m_ScriptPath.string());
			
			m_MethodsTable = m_ComponentTable["methods"];
		}

		void OnStart()
		{
			if (m_MethodsTable["OnStart"].valid())
			{
				m_MethodsTable["OnStart"](m_ComponentTable);
			}
		}

		void OnUpdate(float deltatime)
		{
			if (m_MethodsTable["OnUpdate"].valid())
			{
				m_MethodsTable["OnUpdate"](m_ComponentTable,deltatime);
			}
		}

		void OnEnd()
		{
			if (m_MethodsTable["OnEnd"].valid())
			{
				m_MethodsTable["OnEnd"](m_ComponentTable);
			}
		}

		void OnFixedUpdate()
		{
			if (m_MethodsTable["OnFixedUpdate"].valid())
			{
				m_MethodsTable["OnFixedUpdate"](m_ComponentTable);
			}
		}

		void OnValidate()
		{
			if (m_MethodsTable["OnValidate"].valid())
			{
				m_MethodsTable["OnValidate"](m_ComponentTable);
			}
		}

		void UI() override
		{

		}

		std::string ToString() override
		{
			return "Script Component";
		}

	private:
		sol::state m_lua;
		std::filesystem::path m_ScriptPath;
		sol::table m_MethodsTable;
		sol::table m_ComponentTable;
 	};
}