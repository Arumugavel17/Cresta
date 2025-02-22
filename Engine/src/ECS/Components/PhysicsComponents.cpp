#include "PhysicsComponents.hpp"
#include "ECS/Entity.hpp"

namespace Cresta
{
	void Rigidbody::OnComponentAdded(Entity& entity)
	{
		Scene::AssignPhysicsBody(entity.GetUUID());
		Scene::AddRigidBody(entity.GetComponent<IDComponent>().m_ID);
		CRESTA_INFO("Rigidbody OnComponentAdded");
	}

	void BoxCollider::OnComponentAdded(Entity& entity)
	{
		Scene::AssignPhysicsBody(entity.GetUUID());
		Scene::AddCollider(entity.GetComponent<IDComponent>().m_ID, m_Shape);
		CRESTA_INFO("BoxCollider OnComponentAdded");
	}

	void SphereCollider::OnComponentAdded(Entity& entity)
	{
		Scene::AssignPhysicsBody(entity.GetUUID());
		Scene::AddCollider(entity.GetComponent<IDComponent>().m_ID, m_Shape);
		CRESTA_INFO("SphereCollider OnComponentAdded");
	}

	void CapsuleCollider::OnComponentAdded(Entity& entity)
	{
		Scene::AssignPhysicsBody(entity.GetUUID());
		Scene::AddCollider(entity.GetComponent<IDComponent>().m_ID, m_Shape);
		CRESTA_INFO("CapsuleCollider OnComponentAdded");
	}

	void MeshCollider::OnComponentAdded(Entity& entity)
	{
		Scene::AssignPhysicsBody(entity.GetUUID());
		Scene::AddCollider(entity.GetComponent<IDComponent>().m_ID, m_Shape);
		CRESTA_INFO("MeshCollider OnComponentAdded");
	}

	void Rigidbody::OnComponentRemoved(Entity& entity)
	{
		Scene::RemoveRigidBody(entity.GetUUID());
		CRESTA_INFO("Rigidbody OnComponentRemoved");
	}

	void BoxCollider::OnComponentRemoved(Entity& entity)
	{
		Scene::RemoveCollider(entity.GetUUID());
		CRESTA_INFO("BoxCollider OnComponentRemoved");
	}

	void SphereCollider::OnComponentRemoved(Entity& entity)
	{
		Scene::RemoveCollider(entity.GetUUID());
		CRESTA_INFO("SphereCollider OnComponentRemoved");
	}

	void CapsuleCollider::OnComponentRemoved(Entity& entity)
	{
		Scene::RemoveCollider(entity.GetUUID());
		CRESTA_INFO("CapsuleCollider OnComponentRemoved");
	}

	void MeshCollider::OnComponentRemoved(Entity& entity)
	{
		Scene::RemoveCollider(entity.GetUUID());
		CRESTA_INFO("MeshCollider OnComponentRemoved");
	}
}