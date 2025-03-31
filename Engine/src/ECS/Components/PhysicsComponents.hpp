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
		void SetGravityFactor();

	private:
		glm::vec3 m_Velocity = glm::vec3(0.0f);
		glm::vec3 m_CenterOfMass = glm::vec3(0.0f);
		float m_Density = 0.1f;
		bool m_IsStatic = false;

		float m_GravityFactor = 1;
		float m_Mass = 0;

		UUID m_EnityID;
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

		glm::quat GetRotation() { return m_LocalRotation; }
		glm::vec3 GetSize()		{ return m_LocalScale;    }

		void SetTrigger(bool isTrigger) { m_IsTrigger = isTrigger; }
		bool IsTrigger() { return m_IsTrigger; }

		void SetRotation(const glm::quat& rotation) { m_LocalRotation = rotation;	Revaluate(p_Entity, true); }
		void SetScale(const glm::vec3& size)			{ m_LocalScale = size;			Revaluate(p_Entity, true); }

		std::string ToString() override { return "Box Collider"; }

		static void Revaluate(Entity* entity, bool reflectphysics = false);

		void UI() override;
		void OnStart() override;
		void OnRender() override;
		void OnEnd();
		void OnComponentAdded() override;
		void OnComponentRemoved() override;

		glm::mat4 GetTransform() const
		{
			return glm::translate(glm::mat4(1.0f), m_LocalCenter)
				* glm::toMat4(m_Rotation)
				* glm::scale(glm::mat4(1.0f), m_Scale);
		}

	private:
		bool m_IsTrigger = false;

		glm::quat m_Rotation;
		glm::vec3 m_Scale;

		glm::vec3 m_LocalCenter;
		glm::quat m_LocalRotation;
		glm::vec3 m_LocalScale;

		glm::vec3 rotation = { 0.0f,0.0f ,0.0f };
	};

	class SphereCollider : public Collider
	{
	public:
		SphereCollider(Entity* entity) : Collider(entity, ColliderShape::SphereCollider) {}

		float GetRotation() { return m_Radius; }
		void SetRotation(float radius) { m_Radius = radius; }

		void UI() override;
		std::string ToString() override
		{
			return "Sphere Collider";
		}

		void OnRender() override;
		void OnComponentAdded() override;
		void OnComponentRemoved() override;

	private:
		float m_Radius = 1.0f;
	};

	class CapsuleCollider : public Collider
	{
	public:
		CapsuleCollider(Entity* entity) : Collider(entity, ColliderShape::CapsuleCollider) {}
		void UI() override;
		
		void OnRender() override;
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
		MeshCollider(Entity* entity) : Collider(entity, ColliderShape::MeshCollider) {}
		void UI() override;

		std::string ToString() override
		{
			return "Mesh Collider";
		}
		void OnComponentAdded() override;
		void OnComponentRemoved() override;
	};
}