#pragma once
#include "Components.hpp"

namespace Cresta
{
	class Rigidbody : public Component
	{
	public:
		Rigidbody() = default;

		glm::vec3 Translation = glm::vec3(0.0f);
		glm::vec3 Velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat3 RotationMatrix = glm::mat3(1.0f);
		glm::vec3 RotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);

		void UI() override;

		void SetTranslation(glm::vec3 translation)
		{
			Translation = translation;
		}

		void SetVelocity(const glm::vec3& velocity)
		{
			Velocity = velocity;
		}

		std::string ToString() override
		{
			return "Rigid Body";
		}

		void OnComponentAdded(Entity& entity) override;
		void OnComponentRemoved(Entity& entity) override;
	};

	class Collider : public Component
	{
	public:
		ColliderShape m_Shape;
		Collider() = default;
		Collider(ColliderShape shape) : m_Shape(shape) {}

		void UI() override
		{

		}

		virtual std::string ToString() override
		{
			return "Collider";
		}
	};

	class BoxCollider : public Collider
	{
	public:
		BoxCollider() : Collider(ColliderShape::BoxCollider) {}

		void UI() override;

		std::string ToString() override
		{
			return "Box Collider";
		}

		void OnComponentAdded(Entity& entity) override;
		void OnComponentRemoved(Entity& entity) override;
	};

	class SphereCollider : public Collider
	{
	public:
		SphereCollider() : Collider(ColliderShape::SphereCollider) {}

		void UI() override;
		std::string ToString() override
		{
			return "Sphere Collider";
		}
		void OnComponentAdded(Entity& entity) override;
		void OnComponentRemoved(Entity& entity) override;
	};

	class CapsuleCollider : public Collider
	{
	public:
		CapsuleCollider() : Collider(ColliderShape::CapsuleCollider) {}

		void UI() override;
		
		std::string ToString() override
		{
			return "Capsule Collider";
		}
		void OnComponentAdded(Entity& entity) override;
		void OnComponentRemoved(Entity& entity) override;
	};

	class MeshCollider : public Collider
	{
	public:
		MeshCollider() : Collider(ColliderShape::MeshCollider) {}

		void UI() override;

		std::string ToString() override
		{
			return "Mesh Collider";
		}
		void OnComponentAdded(Entity& entity) override;
		void OnComponentRemoved(Entity& entity) override;
	};
}