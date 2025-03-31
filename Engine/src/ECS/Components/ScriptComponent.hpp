#pragma once
#include "Crestaph.hpp"
#include "Components.hpp"
#include "ECS/Entity.hpp"
#include "WrapperClasses.hpp"

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include<imgui/imgui.h>

namespace Cresta
{
	class ScriptComponent : public ComponentTemplate
	{
	public:
		ScriptComponent(Entity* entiy) : ComponentTemplate(entiy), m_ScriptPath(""), m_EntityWrapper(entiy)
		{
			m_Lua.open_libraries(sol::lib::base);
			m_Lua.set_function("RegisterEntity", [this](sol::table entityTable) {
				this->m_ComponentTable = entityTable;
				});

			m_Lua.new_usertype<SceneWrapper>(
				"Scene",
				sol::constructors<SceneWrapper>(), // Constructor binding
				"CreateEntity", &SceneWrapper::CreateEntity
			);

			m_Lua.new_usertype<EntityWrapper>(
				"Entity",
				sol::constructors<EntityWrapper(Entity*)>(), // Constructor binding
				"GetTransform", &EntityWrapper::GetTransform
			);

			m_Lua.new_usertype<vec3>("vec3",
				sol::constructors<vec3(), vec3(float, float, float)>(),

				"x", &vec3::x,
				"y", &vec3::y,
				"z", &vec3::z,

				sol::meta_function::addition, sol::resolve<vec3(const vec3&) const>(&vec3::operator+),
				sol::meta_function::subtraction, sol::resolve<vec3(const vec3&) const>(&vec3::operator-),
				sol::meta_function::multiplication, sol::resolve<vec3(float) const>(&vec3::operator*),
				sol::meta_function::division, sol::resolve<vec3(float) const>(&vec3::operator/),
				sol::meta_function::equal_to, sol::resolve<bool(const vec3&) const>(&vec3::operator==),

				"dot", &vec3::dot,
				"cross", &vec3::cross,

				"normalize", [](vec3& v) {
					glm::vec3 gv = v;
					gv = glm::normalize(gv);
					v = { gv.x, gv.y, gv.z };
				}
			);

			m_Lua.new_usertype<quat>("quat",
				sol::constructors<quat(), quat(float, float, float, float)>(),

				"w", &quat::w,
				"x", &quat::x,
				"y", &quat::y,
				"z", &quat::z,

				sol::meta_function::multiplication, sol::resolve<quat(const quat&) const>(&quat::operator*),
				sol::meta_function::addition, sol::resolve<quat(const quat&) const>(&quat::operator+),
				sol::meta_function::equal_to, sol::resolve<bool(const quat&) const>(&quat::operator==),
					
				"normalize", [](quat& q) {
					glm::quat gq = q;
					gq = glm::normalize(gq);
					q = { gq.w, gq.x, gq.y, gq.z };
				},

				"fromAxisAngle", sol::factories(&quat::fromAxisAngle) // Static method binding
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

			m_Lua["thisEntity"] = &m_EntityWrapper;
			m_Lua["scene"] = &m_SceneWrapper;

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


		std::string ToString() override
		{
			return "Script Component";
		}

		std::string GetPath()
		{
			return m_ScriptPath.string();
		}

		void UI() override;
	private:
		std::filesystem::path m_ScriptPath;
		std::filesystem::file_time_type m_ScriptWriteTime;

		sol::state m_Lua;
		sol::table m_MethodsTable;
		sol::table m_ComponentTable;

		SceneWrapper m_SceneWrapper;
		EntityWrapper m_EntityWrapper;
 	};
}