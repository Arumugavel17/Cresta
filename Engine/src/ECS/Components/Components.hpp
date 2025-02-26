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
	// Helper to check if a class overrides OnUpdate
	template <typename Base, typename Derived, typename = void>
	struct has_overridden_OnUpdate : std::false_type {}; // Defaults to false if OnUpdate doesn't exist

	template <typename Base, typename Derived>
	struct has_overridden_OnUpdate<Base, Derived, std::void_t<decltype(&Base::OnUpdate), decltype(&Derived::OnUpdate)>> {
		static constexpr bool value = !std::is_same_v<decltype(&Base::OnUpdate), decltype(&Derived::OnUpdate)>;
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
		virtual void OnUpdate() {}
		virtual void OnFixedUpdate() {}
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

		IDComponent() = default; // Ensure default constructibility
		IDComponent(Entity* entity) { std::cout << "Construct"; }
		IDComponent(IDComponent&& other) noexcept
		{
			std::cout << "Move Constructor Called\n";
		}

		// Move Assignment Operator (Debugging Output)
		IDComponent& operator=(IDComponent&& other) noexcept
		{
			std::cout << "Move Assignment Called\n";
			return *this;
		}
		void OnComponentAdded();
		void OnComponentRemoved();

		inline const UUID& GetUUID() const { return m_ID; }
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

		void Save()
		{
			Save_Translation = Translation;
			Save_Rotation = Rotation;
			Save_Scale = Scale;
		}

		void Reset()
		{
			Translation = Save_Translation;
			Rotation = Save_Rotation;
			Scale = Save_Scale;
		}

		void SetRotation(const glm::quat& rotation)
		{
			Rotation = glm::eulerAngles(rotation);
		}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}

		std::string ToString() override
		{
			return "Transform Component";
		}
		
		void UI() override;
		void OnComponentAdded() override;
		void OnComponentRemoved() override;

	public:
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		glm::quat RotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f };

		glm::vec3 Save_Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Save_Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Save_Scale = { 1.0f, 1.0f, 1.0f };
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
