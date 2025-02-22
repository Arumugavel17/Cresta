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
	class Entity;

	class Component
	{
	public:
		template<typename... Dependencies>
		struct Require {};

		virtual ~Component() = default;
		Component() = default;

		virtual void UI() = 0;
		virtual void OnComponentAdded(Entity& entity) {}
		virtual void OnComponentRemoved(Entity& entity) {}
		virtual std::string ToString() = 0;
	};

	class IDComponent
	{
	public:
		UUID m_ID;
	public:
		IDComponent() = default;
		void OnComponentAdded(Entity& entity);
		void OnComponentRemoved(Entity& entity);

		std::string ToString()
		{
			return "IDComponent";
		}
	};

	class TagComponent : public Component 
	{
	public:
		std::string Tag;

		TagComponent() = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}

		void OnComponentAdded(Entity& entity) override;
		void OnComponentRemoved(Entity& entity) override;

		void UI() override
		{

		}
		std::string ToString() override
		{
			return "Tag Component";
		}
	};

	class Transform : public Component 
	{
	public:
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		glm::quat RotationQuat = { 0.0f, 0.0f, 0.0f, 0.0f};

		glm::vec3 Save_Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Save_Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Save_Scale = { 1.0f, 1.0f, 1.0f };

		Transform() = default;
 		Transform(const glm::vec3& translation)
			: Translation(translation) {}

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
		void OnComponentAdded(Entity& entity) override;
		void OnComponentRemoved(Entity& entity) override;
	};

	class CameraComponent : public Component
	{
	public:
		Camera Camera;
		bool Primary = true; // TODO: think about moving to Scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;

		std::string ToString() override
		{
			return "Camera Component";
		}

		void UI() override
		{

		}
		void OnComponentAdded(Entity& entity) override;
		void OnComponentRemoved(Entity& entity) override;
	};
}
