#pragma once
#include "Crestaph.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/Model.hpp"
#include "Renderer/Texture.hpp"
#include "ECS/UUID.hpp"
#include "Core/Physics/PhysicsUtils.hpp"

#include <exception>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Jolt/Physics/Body/BodyID.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Cresta 
{
	class Dispatcher
	{
	public:		
		void Subscribe(const std::string& key, std::function<void(bool)> function)
		{
			Observers[key] = function;
		}

		void UnSubscribe(const std::string& key)
		{
			Observers.erase(key);
		}

		void post(bool value) const
		{
			for (auto& observer : Observers)
			{
				observer.second(value);
			}
		}

	private:
		std::map<std::string, std::function<void(bool)>> Observers;
	};

	// Helper to check if a class overrides OnStart
	template <typename Base, typename Derived, typename = void>
	struct has_overridden_OnStart : std::false_type {}; // Defaults to false if OnUpdate doesn't exist

	template <typename Base, typename Derived>
	struct has_overridden_OnStart <Base, Derived, std::void_t<decltype(&Base::OnStart), decltype(&Derived::OnStart)>> {
		static constexpr bool value = !std::is_same_v<decltype(&Base::OnStart), decltype(&Derived::OnStart)>;
	};

	// Helper to check if a class overrides OnRender
	template <typename Base, typename Derived, typename = void>
	struct has_overridden_OnRender : std::false_type {}; // Defaults to false if OnUpdate doesn't exist

	template <typename Base, typename Derived>
	struct has_overridden_OnRender<Base, Derived, std::void_t<decltype(&Base::OnRender), decltype(&Derived::OnRender)>> {
		static constexpr bool value = !std::is_same_v<decltype(&Base::OnRender), decltype(&Derived::OnRender)>;
	};

	// Helper to check if a class overrides OnUpdate
	template <typename Base, typename Derived, typename = void>
	struct has_overridden_OnUpdate : std::false_type {}; // Defaults to false if OnUpdate doesn't exist

	template <typename Base, typename Derived>
	struct has_overridden_OnUpdate<Base, Derived, std::void_t<decltype(&Base::OnUpdate), decltype(&Derived::OnUpdate)>> {
		static constexpr bool value = !std::is_same_v<decltype(&Base::OnUpdate), decltype(&Derived::OnUpdate)>;
	};

	// Helper to check if a class overrides OnFixedUpdate
	template <typename Base, typename Derived, typename = void>
	struct has_overridden_OnFixedUpdate : std::false_type {}; // Defaults to false if OnFixedUpdate doesn't exist

	template <typename Base, typename Derived>
	struct has_overridden_OnFixedUpdate<Base, Derived, std::void_t<decltype(&Base::OnFixedUpdate), decltype(&Derived::OnFixedUpdate)>> {
		static constexpr bool value = !std::is_same_v<decltype(&Base::OnFixedUpdate), decltype(&Derived::OnFixedUpdate)>;
	};

	// Helper to check if a class overrides OnEnd
	template <typename Base, typename Derived, typename = void>
	struct has_overridden_OnEnd : std::false_type {}; // Defaults to false if OnUpdate doesn't exist

	template <typename Base, typename Derived>
	struct has_overridden_OnEnd <Base, Derived, std::void_t<decltype(&Base::OnEnd), decltype(&Derived::OnEnd)>> {
		static constexpr bool value = !std::is_same_v<decltype(&Base::OnEnd), decltype(&Derived::OnEnd)>;
	};

	class Entity;

	class ComponentTemplate 
	{
	public:
		explicit ComponentTemplate(Entity* entity) : p_Entity(entity) {}
		virtual ~ComponentTemplate() = default;

		ComponentTemplate(ComponentTemplate&& other) noexcept : p_Entity(std::exchange(other.p_Entity, nullptr)) {}
		ComponentTemplate& operator=(ComponentTemplate&& other) noexcept {
			if (this != &other) {
				p_Entity = std::exchange(other.p_Entity, nullptr);
			}
			return *this;
		}

		virtual void UI() = 0;
		virtual void OnStart() {}
		virtual void OnRender() {}
		virtual void OnUpdate() {}
		virtual void OnFixedUpdate() {}
		virtual void OnEnd() {}
		virtual void OnGizmo() {}
		virtual void OnComponentAdded() {}
		virtual void OnComponentRemoved() {}
		virtual std::string ToString() = 0;

		Entity* GetEntity() { return p_Entity; }
		const Entity* GetEntity() const { return p_Entity; }

	protected:
		Entity* p_Entity;
	};

	class IDComponent
	{
	private:
		UUID m_ID;
	public:
		IDComponent()
		{
			std::cout << "Default Construct\n";
		}
		IDComponent(Entity* entity) 
		{ 
			std::cout << "Entity* entity Construct\n"; 
		}
		IDComponent(Entity* entity, uint64_t ID) : m_ID(ID)	
		{
			std::cout << "Entity* entity Construct\n";
		}

		void OnComponentAdded();
		void OnComponentRemoved();

		inline const UUID& GetUUID() const { 
			return m_ID; 
		}
		std::string ToString()
		{
			return "IDComponent";
		}
	};

	class TagComponent : public ComponentTemplate
	{
	public:
		std::string Tag;
		TagComponent(Entity* entity, const std::string& tag) : ComponentTemplate(entity),Tag(tag) {}

		void UI() override {}
		void OnComponentAdded() override;
		void OnComponentRemoved() override;
		std::string ToString() override
		{
			return "Tag Component";
		}
	};

	class Transform : public ComponentTemplate 
	{
	public:
		Transform(Entity* entity) : ComponentTemplate(entity) {}

		inline void SetPosition(const glm::vec3& Position)		{ m_Translation = Position;	OnValidate.post(false);	}
		inline void SetScale(const glm::vec3& Scale)			{ m_Scale = Scale;			OnValidate.post(false);	}
		inline void SetRotation(const glm::quat& rotation)		
		{
			m_Rotation = rotation;	
			this->rotation = glm::degrees(glm::eulerAngles(m_Rotation));
			
			OnValidate.post(false);	
		}

		inline constexpr glm::vec3& GetPosition() const { return *(new glm::vec3(m_Translation)); }
		inline constexpr glm::quat& GetRotation() const { return *(new glm::quat(m_Rotation));	}
		inline constexpr glm::vec3& GetScale()	  const { return *(new glm::vec3(m_Scale));		}

		glm::mat4 GetTransform() const
		{
			return glm::translate(glm::mat4(1.0f), m_Translation)
				* glm::toMat4(m_Rotation)
				* glm::scale(glm::mat4(1.0f), m_Scale);
		}

		std::string ToString() override { return "Transform Component"; }

		void UI() override;
		void OnComponentAdded() override;
		void OnComponentRemoved() override;

	private:
		glm::vec3 m_Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = { 0.0f,0.0f, 0.0f };
		glm::quat m_Rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
		glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };
	public:
		Dispatcher OnValidate;
	};

	class CameraComponent : public ComponentTemplate
	{
	public:
		CameraComponent(Entity* entity) : ComponentTemplate(entity) {}

		Camera Camera;
		bool Primary = true; // TODO: think about moving to Scene
		bool FixedAspectRatio = false;


		std::string ToString() override
		{
			return "Camera Component";
		}

		void UI() override {}
		void OnComponentAdded() override;
		void OnComponentRemoved() override;
	};
}
