#include "PhysicsComponents.hpp"
#include "Renderer/PrimitiveMeshes.hpp"
#include "ECS/Entity.hpp"
#include "Renderer/Renderer.hpp"

namespace Cresta
{
	void Rigidbody::OnStart()
	{
		CRESTA_PROFILE_FUNCTION();

		Physics::SetBodyPosition(m_EnityID, p_Entity->GetComponent<Transform>().GetPosition());
		Physics::SetBodyRotation(m_EnityID, p_Entity->GetComponent<Transform>().GetRotation());
	}

	void Rigidbody::OnFixedUpdate()
	{
		CRESTA_PROFILE_FUNCTION();

		p_Entity->GetComponent<Transform>().SetPosition(
			Physics::GetBodyPosition(m_EnityID)
		);

		p_Entity->GetComponent<Transform>().SetRotation(
			Physics::GetBodyRotation(m_EnityID)
		);
	}

	void Rigidbody::OnComponentAdded()
	{
		m_EnityID = p_Entity->GetUUID();
		Scene::AddRigidBody(m_EnityID);
	}

	BoxCollider::BoxCollider(Entity* entity) : Collider(entity, ColliderShape::BoxCollider) {}

	void BoxCollider::OnComponentAdded()
	{
		Scene::AddCollider(p_Entity->GetUUID(), m_Shape);
		Transform& transform = p_Entity->GetComponent<Transform>();
		transform.OnValidate.Subscribe(typeid(BoxCollider).name(), [entity = p_Entity](bool reflectphysics = false)
			{
				BoxCollider::Revaluate(entity, reflectphysics);
			});

		m_Rotation = glm::eulerAngles(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
		m_Scale = glm::vec3(1.0f);
		m_LocalCenter = transform.GetPosition();
		m_LocalRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		m_LocalScale = glm::vec3(1.0f);

		Revaluate(p_Entity,true);
	}

	void BoxCollider::Revaluate(Entity* entity, bool reflectphysics )
	{
		CRESTA_PROFILE_FUNCTION();

		auto& collider = entity->GetComponent<BoxCollider>();
		auto& transform = entity->GetComponent<Transform>();

		// Apply transformation to the stored local values
		glm::vec3 parentPosition = transform.GetPosition();
		glm::quat parentRotation = transform.GetRotation(); // Parent rotation
		glm::vec3 parentScale = transform.GetScale();

		collider.m_LocalCenter = parentPosition;
		collider.m_Rotation = parentRotation * collider.m_LocalRotation;

		collider.m_Scale = parentScale * collider.m_LocalScale;

		if (reflectphysics)
		{
			Physics::SetBodyPosition(entity->GetUUID(), collider.m_LocalCenter);
			Physics::SetBodyRotation(entity->GetUUID(), collider.m_Rotation);
		}

		if (std::abs(collider.m_Scale.x) > 0.1f && std::abs(collider.m_Scale.y) > 0.1f && std::abs(collider.m_Scale.z) > 0.1f)
		{
			Physics::SetBodyShapeScale(entity->GetUUID(), collider.m_Scale);
		}
	}

	void BoxCollider::OnStart()
	{
		CRESTA_PROFILE_FUNCTION();
		std::cout << m_LocalRotation << "\n";

		if (std::abs(m_Scale.x) > 0.1f && std::abs(m_Scale.y) > 0.1f && std::abs(m_Scale.z) > 0.1f)
		{
			Physics::SetBodyShapeScale(p_Entity->GetUUID(), m_Scale);
		}
	}

	void BoxCollider::OnRender()
	{
		CRESTA_PROFILE_FUNCTION();
		Primitive::DrawCubeGuizmo(GetTransform());
	}

	void BoxCollider::OnEnd()
	{
	}
	
	void SphereCollider::OnComponentAdded()
	{
		Scene::AddCollider(p_Entity->GetComponent<IDComponent>().GetUUID(), m_Shape);
	}

	void CapsuleCollider::OnRender()
	{
		Primitive::DrawCapsuleGuizmo(p_Entity->GetComponent<Transform>().GetTransform());
	}

	void CapsuleCollider::OnComponentAdded()
	{
		Scene::AddCollider(p_Entity->GetComponent<IDComponent>().GetUUID(), m_Shape);
	}

	void MeshCollider::OnComponentAdded()
	{
		Scene::AddCollider(p_Entity->GetComponent<IDComponent>().GetUUID(), m_Shape);
	}

	//ComponentRemoved///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Rigidbody::OnComponentRemoved()
	{
		Scene::RemoveRigidBody(p_Entity->GetUUID());
	}

	void Rigidbody::SetGravityFactor()
	{
		Physics::SetGravityFactor(m_EnityID, m_GravityFactor);
	}

	void BoxCollider::OnComponentRemoved()
	{
		Scene::RemoveCollider(p_Entity->GetUUID());

		p_Entity->GetComponent<Transform>().OnValidate.UnSubscribe(typeid(BoxCollider).name());
	}

	void SphereCollider::OnComponentRemoved()
	{
		Scene::RemoveCollider(p_Entity->GetUUID());
	}

	void SphereCollider::OnRender()
	{
		glm::vec3 position = p_Entity->GetComponent<Transform>().GetPosition();
		glm::quat rotation = p_Entity->GetComponent<Transform>().GetRotation();
		Primitive::DrawSphereGuizmo(position,rotation, m_Radius);
	}

	void CapsuleCollider::OnComponentRemoved()
	{
		Scene::RemoveCollider(p_Entity->GetUUID());
	}

	void MeshCollider::OnComponentRemoved()
	{
		Scene::RemoveCollider(p_Entity->GetUUID());
	}
}