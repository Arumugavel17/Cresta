#pragma once
#include "Components.hpp"
#include "Core/Physics/Physics.hpp"

namespace Cresta
{
	class Rigidbody : public ComponentTemplate
	{
	public:
		Rigidbody(Entity* entity) : ComponentTemplate(entity) {}
		void UI() override;

		void SetVelocity(const glm::vec3& velocity)
		{
			m_Velocity = velocity;
		}

		std::string ToString() override
		{
			return "Rigid Body";
		}

		void OnComponentAdded() override;
		void OnComponentRemoved() override;

	private:
		glm::vec3 m_Velocity = glm::vec3(0.0f);
		glm::vec3 m_CenterOfMass = glm::vec3(0.0f);
		float m_Density = 0.1f;
		bool m_IsStatic = false;
	};

	class Collider : public ComponentTemplate
	{
	public:
		ColliderShape m_Shape;
		Collider(Entity* entity, ColliderShape shape) : ComponentTemplate(entity), m_Shape(shape) {}

		virtual void UI() override {}
		
		virtual std::string ToString() override
		{
			return "Collider";
		}
	};

	class BoxCollider : public Collider
	{
	public:
		BoxCollider(Entity* entity) : Collider(entity,ColliderShape::BoxCollider) {}
		void UI() override;

		void SetCenter(const glm::vec3& center)
		{
			m_Center.x = center.x;
			m_Center.y = center.y;
			m_Center.z = center.z;
		}

		void SetSize(const glm::vec3& size)
		{
			m_Size.x = size.x;
			m_Size.y = size.y;
			m_Size.z = size.z;
		}

		std::string ToString() override
		{
			return "Box Collider";
		}

		void OnComponentAdded() override;
		void OnComponentRemoved() override;
	private:
		glm::vec3 m_Center = glm::vec3(0.0f);
		glm::vec3 m_Size = glm::vec3(0.0f);
	};

	class SphereCollider : public Collider
	{
	public:
		SphereCollider(Entity* entity) : Collider(entity, ColliderShape::BoxCollider) {}
		void UI() override;
		std::string ToString() override
		{
			return "Sphere Collider";
		}
		void OnComponentAdded() override;
		void OnComponentRemoved() override;
	};

	class CapsuleCollider : public Collider
	{
	public:
		CapsuleCollider(Entity* entity) : Collider(entity, ColliderShape::BoxCollider) {}
		void UI() override;
		
		std::string ToString() override
		{
			return "Capsule Collider";
		}
		void OnComponentAdded() override;
		void OnComponentRemoved() override;
	};

	class MeshCollider : public Collider
	{
	public:
		MeshCollider(Entity* entity) : Collider(entity, ColliderShape::BoxCollider) {}
		void UI() override;

		std::string ToString() override
		{
			return "Mesh Collider";
		}
		void OnComponentAdded() override;
		void OnComponentRemoved() override;
	};
}