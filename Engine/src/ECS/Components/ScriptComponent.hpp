#pragma once
#include "Crestaph.hpp"
#include "Components.hpp"
#include "ScriptEngine/LuaManager.hpp"

namespace Cresta
{
	class ScriptComponent : public ComponentTemplate
	{
	public:
		ScriptComponent(Entity* entiy) : ComponentTemplate(entiy), m_ScriptPath("")
		{
			m_lua.open_libraries(sol::lib::base);
		}

		void SetPath(const std::string& path)
		{
			m_ScriptPath = path;

			m_lua.set_function("RegisterEntity", [this](sol::table entityTable) {
				this->m_ComponentTable = entityTable;
				});

			m_lua.script_file(m_ScriptPath.string());

			m_MethodsTable = m_ComponentTable["methods"];
		}

		void OnStart()
		{
			if (m_ScriptPath.empty())
			{
				return;
			}

			if (m_MethodsTable["OnStart"].valid())
			{
				m_MethodsTable["OnStart"](m_ComponentTable);
			}
		}

		void OnUpdate()
		{
			if (m_ScriptPath.empty())
			{
				return;
			}

			if (m_MethodsTable["OnUpdate"].valid())
			{
				m_MethodsTable["OnUpdate"](m_ComponentTable);
			}
		}

		void OnEnd()
		{
			if (m_ScriptPath.empty())
			{
				return;
			}

			if (m_MethodsTable["OnEnd"].valid())
			{
				m_MethodsTable["OnEnd"](m_ComponentTable);
			}
		}

		void OnFixedUpdate()
		{
			if (m_ScriptPath.empty())
			{
				return;
			}

			if (m_MethodsTable["OnFixedUpdate"].valid())
			{
				m_MethodsTable["OnFixedUpdate"](m_ComponentTable);
			}
		}

		void OnValidate()
		{
			if (m_ScriptPath.empty())
			{
				return;
			}

			if (m_MethodsTable["OnValidate"].valid())
			{
				m_MethodsTable["OnValidate"](m_ComponentTable);
			}
		}

		void UI() override
		{
			ImGui::Text("Script Component");

			char buffer[128];
			std::strncpy(buffer, m_ScriptPath.string().c_str(), sizeof(buffer) - 1);
			buffer[sizeof(buffer) - 1] = '\0';

			if (ImGui::InputText("TextPath", buffer, sizeof(buffer), ImGuiInputTextFlags_ReadOnly))
			{
				SetPath(buffer);
			}

			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_PATH");
				if (payload != nullptr)
				{
					std::string tempString(static_cast<const char*>(payload->Data), payload->DataSize);
					SetPath(tempString);
				}
				ImGui::EndDragDropTarget();
			}
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