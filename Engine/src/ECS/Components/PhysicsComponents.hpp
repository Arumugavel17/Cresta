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

		void OnStart() override;
		void OnFixedUpdate() override;
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
		BoxCollider(Entity* entity);
		void UI() override;

		void SetCenter(const glm::vec3& center)
		{
			m_Center = center;
		}

		void SetRotation(const glm::vec3& rotation)
		{
			m_Rotation = rotation;
		}

		void SetSize(const glm::vec3& size)
		{
			m_Scale = size;
		}

		std::string ToString() override
		{
			return "Box Collider";
		}

		static void Revaluate(Entity* entity);

		void OnStart() override;
		void OnUpdate() override;
		void OnFixedUpdate() override;
		void OnComponentAdded() override;
		void OnComponentRemoved() override;

		glm::mat4 GetTransform() const
		{
			return glm::translate(glm::mat4(1.0f), m_Center)
				* glm::toMat4(glm::quat(m_Rotation))
				* glm::scale(glm::mat4(1.0f), m_Scale);
		}


	private:
		
		glm::vec3 m_Rotation;
		glm::vec3 m_Center;
		glm::vec3 m_Scale;

		glm::vec3 OffestLocalCenter;
		glm::vec3 OffestLocalRotation;
		glm::vec3 OffestLocalScale;
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