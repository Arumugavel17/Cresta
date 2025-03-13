#pragma once
#include "Crestaph.hpp"
#include "Components.hpp"
#include "ScriptEngine/LuaManager.hpp"
#include "RendererComponents.hpp"
#include "PhysicsComponents.hpp"
#include "ECS/Entity.hpp"


namespace Cresta
{
	struct vec3
	{
		float x, y, z;
	};
	
	struct quat
	{
		float x, y, z, w;
	};

	inline vec3 CreateVec3(float x, float y, float z)
	{
		return vec3(x,y,z);
	}

	class TransformWrapper
	{
	public:

		inline void SetPosition(const vec3& Position) { TransformComponent->SetPosition({ Position.x,Position.y,Position.z }); }
		inline void SetScale(const vec3& Scale) { TransformComponent->SetScale({ Scale.x,Scale.y,Scale.z }); }
		inline void SetRotation(const quat& Position) { TransformComponent->SetRotation({ Position.x,Position.y,Position.z,Position.w }); }

		inline constexpr vec3& GetPosition() const 
		{ 
			glm::vec3 position = TransformComponent->GetPosition();
			return *(new vec3(position.x, position.y, position.z));
		}

		inline constexpr vec3& GetRotation() const 
		{
			glm::vec3 rotation = TransformComponent->GetRotation();
			return *(new vec3(rotation.x, rotation.y, rotation.z));
		}

		inline constexpr vec3& GetScale() const 
		{
			glm::vec3 scale = TransformComponent->GetScale();
			return *(new vec3(scale.x, scale.y, scale.z));
		}

		Transform* TransformComponent;
	};

	class EntityWrapper
	{
	public:
		EntityWrapper(Entity* entity) : p_Entity(entity)
		{

		}

		TransformWrapper& GetTransform()
		{
			m_TransformWrapper.TransformComponent = &p_Entity->GetComponent<Transform>();
			return m_TransformWrapper;
		}

	private:
		Entity* p_Entity;
		TransformWrapper m_TransformWrapper;
	};

	class ScriptComponent : public ComponentTemplate
	{
	public:

		ScriptComponent(Entity* entiy) : ComponentTemplate(entiy), m_ScriptPath(""), m_EntityWrapper(p_Entity)
		{
			m_Lua.open_libraries(sol::lib::base);
			m_Lua.set_function("RegisterEntity", [this](sol::table entityTable) {
				this->m_ComponentTable = entityTable;
				});

			m_Lua.new_usertype<EntityWrapper>(
				"Entity",
				sol::constructors<EntityWrapper(Entity*)>(), // Constructor binding
				"GetTransform", &EntityWrapper::GetTransform
			);

			m_Lua.new_usertype<vec3>(
				"vec3",
				sol::constructors<vec3(), vec3(float, float, float)>(),
				"x", &vec3::x,
				"y", &vec3::y,
				"z", &vec3::z
			);

			m_Lua.new_usertype<quat>("quat",
				sol::constructors<quat(), quat(float, float, float, float)>(),
				"x", &quat::x,
				"y", &quat::y,
				"z", &quat::z,
				"w", &quat::w
			);

			m_Lua.new_usertype<TransformWrapper>(
				"Transform",
				"SetPosition",	&TransformWrapper::SetPosition,
				"SetRotation",	&TransformWrapper::SetRotation,
				"SetScale",		&TransformWrapper::SetScale,
				"GetPosition",	&TransformWrapper::GetPosition,
				"GetRotation",	&TransformWrapper::GetRotation,
				"GetScale",		&TransformWrapper::GetScale
			);

			m_Lua["entity"] = &m_EntityWrapper;

			m_Lua.set_function("CreateVec3", CreateVec3);
		}

		void OnRender() override
		{
			if (m_ScriptPath.empty())
			{
				return;
			}

			auto currentWriteTime = std::filesystem::last_write_time(m_ScriptPath);
			if (currentWriteTime != m_ScriptWriteTime)
			{
				m_Lua.script_file(m_ScriptPath.string());
				m_ScriptWriteTime = currentWriteTime; // Update last known modification time
			}
		}

		void SetPath(const std::string& path)
		{
			m_ScriptPath = path;
			m_Lua.script_file(m_ScriptPath.string());
			m_ScriptWriteTime = std::filesystem::last_write_time(m_ScriptPath); // Get initial modification time
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
		std::filesystem::path m_ScriptPath;
		std::filesystem::file_time_type m_ScriptWriteTime;

		sol::state m_Lua;
		sol::table m_MethodsTable;
		sol::table m_ComponentTable;

		EntityWrapper m_EntityWrapper;
 	};
}