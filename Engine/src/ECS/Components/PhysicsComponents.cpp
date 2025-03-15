#include "PhysicsComponents.hpp"
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

		m_Rotation = glm::vec3(0.0f);
		m_Center = glm::vec3(0.0f);
		m_Scale = glm::vec3(1.0f);

		Transform& transform = p_Entity->GetComponent<Transform>();
		transform.OnValidate.Subscribe(typeid(BoxCollider).name(), [entity = p_Entity](bool reflectphysics = false)
			{
				BoxCollider::Revaluate(entity, reflectphysics);
			});

		m_Center = transform.GetPosition();
		m_LocalRotation = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);
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

		collider.m_Center = parentPosition;
		collider.m_Rotation = parentRotation * collider.m_LocalRotation;
		collider.m_Scale = parentScale * collider.m_LocalScale;

		if (reflectphysics)
		{
			if ((uint32_t)entity == 1)
			{
				std::cout << collider.m_Center << " " << parentPosition << " " << "\n";
			}
			Physics::SetBodyPosition(entity->GetUUID(), collider.m_Center);
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

	void BoxCollider::OnComponentRemoved()
	{
		Scene::RemoveCollider(p_Entity->GetUUID());

		p_Entity->GetComponent<Transform>().OnValidate.UnSubscribe(typeid(BoxCollider).name());
	}

	void SphereCollider::OnComponentRemoved()
	{
		Scene::RemoveCollider(p_Entity->GetUUID());
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