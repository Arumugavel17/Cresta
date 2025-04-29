#pragma once
#include "Crestaph.hpp"
#include "Components.hpp"
#include "ECS/Entity.hpp"
#include "WrapperClasses.hpp"
#include "Core/Input.hpp"

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include<imgui/imgui.h>

namespace Cresta
{
	static void BindKeyEnum(sol::state& lua)
	{
		sol::table keyTable = lua.create_table();

		// General keys
		keyTable["Space"] = Key::Space;
		keyTable["Apostrophe"] = Key::Apostrophe;
		keyTable["Comma"] = Key::Comma;
		keyTable["Minus"] = Key::Minus;
		keyTable["Period"] = Key::Period;
		keyTable["Slash"] = Key::Slash;

		// Number keys
		keyTable["D0"] = Key::D0;
		keyTable["D1"] = Key::D1;
		keyTable["D2"] = Key::D2;
		keyTable["D3"] = Key::D3;
		keyTable["D4"] = Key::D4;
		keyTable["D5"] = Key::D5;
		keyTable["D6"] = Key::D6;
		keyTable["D7"] = Key::D7;
		keyTable["D8"] = Key::D8;
		keyTable["D9"] = Key::D9;

		// Symbols
		keyTable["Semicolon"] = Key::Semicolon;
		keyTable["Equal"] = Key::Equal;

		// Alphabet keys
		keyTable["A"] = Key::A;
		keyTable["B"] = Key::B;
		keyTable["C"] = Key::C;
		keyTable["D"] = Key::D;
		keyTable["E"] = Key::E;
		keyTable["F"] = Key::F;
		keyTable["G"] = Key::G;
		keyTable["H"] = Key::H;
		keyTable["I"] = Key::I;
		keyTable["J"] = Key::J;
		keyTable["K"] = Key::K;
		keyTable["L"] = Key::L;
		keyTable["M"] = Key::M;
		keyTable["N"] = Key::N;
		keyTable["O"] = Key::O;
		keyTable["P"] = Key::P;
		keyTable["Q"] = Key::Q;
		keyTable["R"] = Key::R;
		keyTable["S"] = Key::S;
		keyTable["T"] = Key::T;
		keyTable["U"] = Key::U;
		keyTable["V"] = Key::V;
		keyTable["W"] = Key::W;
		keyTable["X"] = Key::X;
		keyTable["Y"] = Key::Y;
		keyTable["Z"] = Key::Z;

		// Brackets and Backslash
		keyTable["LeftBracket"] = Key::LeftBracket;
		keyTable["Backslash"] = Key::Backslash;
		keyTable["RightBracket"] = Key::RightBracket;
		keyTable["GraveAccent"] = Key::GraveAccent;

		// Non-US Keys
		keyTable["World1"] = Key::World1;
		keyTable["World2"] = Key::World2;

		// Function keys
		keyTable["Escape"] = Key::Escape;
		keyTable["Enter"] = Key::Enter;
		keyTable["Tab"] = Key::Tab;
		keyTable["Backspace"] = Key::Backspace;
		keyTable["Insert"] = Key::Insert;
		keyTable["Delete"] = Key::Delete;
		keyTable["Right"] = Key::Right;
		keyTable["Left"] = Key::Left;
		keyTable["Down"] = Key::Down;
		keyTable["Up"] = Key::Up;
		keyTable["PageUp"] = Key::PageUp;
		keyTable["PageDown"] = Key::PageDown;
		keyTable["Home"] = Key::Home;
		keyTable["End"] = Key::End;
		keyTable["CapsLock"] = Key::CapsLock;
		keyTable["ScrollLock"] = Key::ScrollLock;
		keyTable["NumLock"] = Key::NumLock;
		keyTable["PrintScreen"] = Key::PrintScreen;
		keyTable["Pause"] = Key::Pause;

		// Function Keys F1-F25
		keyTable["F1"] = Key::F1;
		keyTable["F2"] = Key::F2;
		keyTable["F3"] = Key::F3;
		keyTable["F4"] = Key::F4;
		keyTable["F5"] = Key::F5;
		keyTable["F6"] = Key::F6;
		keyTable["F7"] = Key::F7;
		keyTable["F8"] = Key::F8;
		keyTable["F9"] = Key::F9;
		keyTable["F10"] = Key::F10;
		keyTable["F11"] = Key::F11;
		keyTable["F12"] = Key::F12;
		keyTable["F13"] = Key::F13;
		keyTable["F14"] = Key::F14;
		keyTable["F15"] = Key::F15;
		keyTable["F16"] = Key::F16;
		keyTable["F17"] = Key::F17;
		keyTable["F18"] = Key::F18;
		keyTable["F19"] = Key::F19;
		keyTable["F20"] = Key::F20;
		keyTable["F21"] = Key::F21;
		keyTable["F22"] = Key::F22;
		keyTable["F23"] = Key::F23;
		keyTable["F24"] = Key::F24;
		keyTable["F25"] = Key::F25;

		// Keypad keys
		keyTable["KP0"] = Key::KP0;
		keyTable["KP1"] = Key::KP1;
		keyTable["KP2"] = Key::KP2;
		keyTable["KP3"] = Key::KP3;
		keyTable["KP4"] = Key::KP4;
		keyTable["KP5"] = Key::KP5;
		keyTable["KP6"] = Key::KP6;
		keyTable["KP7"] = Key::KP7;
		keyTable["KP8"] = Key::KP8;
		keyTable["KP9"] = Key::KP9;
		keyTable["KPDecimal"] = Key::KPDecimal;
		keyTable["KPDivide"] = Key::KPDivide;
		keyTable["KPMultiply"] = Key::KPMultiply;
		keyTable["KPSubtract"] = Key::KPSubtract;
		keyTable["KPAdd"] = Key::KPAdd;
		keyTable["KPEnter"] = Key::KPEnter;
		keyTable["KPEqual"] = Key::KPEqual;

		// Modifier keys
		keyTable["LeftShift"] = Key::LeftShift;
		keyTable["LeftControl"] = Key::LeftControl;
		keyTable["LeftAlt"] = Key::LeftAlt;
		keyTable["LeftSuper"] = Key::LeftSuper;
		keyTable["RightShift"] = Key::RightShift;
		keyTable["RightControl"] = Key::RightControl;
		keyTable["RightAlt"] = Key::RightAlt;
		keyTable["RightSuper"] = Key::RightSuper;
		keyTable["Menu"] = Key::Menu;

		lua["Key"] = keyTable; // Assign table to Lua
	}

	static void BindMouseCodesToLua(sol::state& lua) {
		sol::table mouseTable = lua.create_table();

		// Mouse buttons
		mouseTable["Button0"] = Mouse::Button0;
		mouseTable["Button1"] = Mouse::Button1;
		mouseTable["Button2"] = Mouse::Button2;
		mouseTable["Button3"] = Mouse::Button3;
		mouseTable["Button4"] = Mouse::Button4;
		mouseTable["Button5"] = Mouse::Button5;
		mouseTable["Button6"] = Mouse::Button6;
		mouseTable["Button7"] = Mouse::Button7;

		// Aliases
		mouseTable["ButtonLast"]	= Mouse::ButtonLast;
		mouseTable["ButtonLeft"]	= Mouse::ButtonLeft;
		mouseTable["ButtonRight"]	= Mouse::ButtonRight;
		mouseTable["ButtonMiddle"]	= Mouse::ButtonMiddle;

		lua["Mouse"] = mouseTable; // Assign table to Lua
	}

	static void BindInputClass(sol::state& lua)
	{
		lua.new_usertype<Input>("Input",
			sol::no_constructor,  // Prevent instantiation if it's a static class
			"GetKeyDown", &Input::GetKeyDown,
			"GetKeyUp", &Input::GetKeyUp,
			"GetMouseDown", &Input::GetMouseDown,
			"GetMouseUp", &Input::GetMouseUp,
			"GetMousePosition", []() {
				auto pos = Input::GetMousePosition();
				return sol::as_table(std::vector<float>{pos.first, pos.second});
			}
		);
	}

	class ScriptComponent : public ComponentTemplate
	{
	public:
		ScriptComponent(Entity* entiy) : ComponentTemplate(entiy), m_ScriptPath("")
		{
			m_EntityWrapper = CreateScope<EntityWrapper>(entiy);
			m_SceneWrapper =  CreateScope<SceneWrapper>();

			m_Lua.open_libraries(sol::lib::base);
			m_Lua.set_function("RegisterEntity",  [this](sol::table entityTable) {
				this->m_ComponentTable = entityTable;
				});

			m_Lua.new_usertype<SceneWrapper>(
				"Scene",
				sol::constructors<SceneWrapper>(), // Constructor binding
				"CreateEntity", &SceneWrapper::CreateEntity,
				"DestroyEntity", &SceneWrapper::DestroyEntity
			);

			m_Lua.new_usertype<EntityWrapper>(
				"Entity",
				"GetTransform", &EntityWrapper::GetTransform,
				"AttachMeshRenderer", &EntityWrapper::AttachMeshRenderer,
				"AttachBoxCollider", &EntityWrapper::AttachBoxCollider,
				"AttachScript", &EntityWrapper::AttachScript,
				"PrintHandle", &EntityWrapper::PrintHandle
			);

			m_Lua.new_usertype<MeshRendererWrapper>(
				"MeshRenderer",
				sol::constructors<MeshRendererWrapper>(), // Constructor binding
				"SetPath", &MeshRendererWrapper::SetPath
			);

			m_Lua.new_usertype<BoxColliderWrapper>(
				"BoxColliderWrapper",
				sol::constructors<BoxColliderWrapper>(), // Constructor binding
				"SetPath", &BoxColliderWrapper::SetScale,
				"SetRotation", &BoxColliderWrapper::SetRotation
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

			m_Lua.new_usertype<Animation>("Animation");

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

			BindKeyEnum(m_Lua);
			BindInputClass(m_Lua);

			m_Lua.new_usertype<TransformWrapper>(
				"Transform",
				"SetPosition",	&TransformWrapper::SetPosition,
				"SetRotation",	&TransformWrapper::SetRotation,
				"SetScale",		&TransformWrapper::SetScale,
				"GetPosition",	&TransformWrapper::GetPosition,
				"GetRotation",	&TransformWrapper::GetRotation,
				"GetScale",		&TransformWrapper::GetScale
			);

			m_Lua["thisEntity"] = m_EntityWrapper.get();
			m_Lua["scene"] = m_SceneWrapper.get();

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

		Scope<SceneWrapper> m_SceneWrapper;
		Scope<EntityWrapper> m_EntityWrapper;
 	};
}