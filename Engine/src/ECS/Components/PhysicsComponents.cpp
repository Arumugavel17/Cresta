#include "PhysicsComponents.hpp"
#include "ECS/Entity.hpp"

namespace Cresta
{
	void Rigidbody::OnComponentAdded(Entity& entity)
	{
		Scene::AssignPhysicsBody(entity.GetUUID());
		Scene::AddRigidBody(entity.GetComponent<IDComponent>().m_ID);
	}

	void BoxCollider::OnComponentAdded(Entity& entity)
	{
		Scene::AssignPhysicsBody(entity.GetUUID());
		Scene::AddCollider(entity.GetComponent<IDComponent>().m_ID, m_Shape);
	}

	void SphereCollider::OnComponentAdded(Entity& entity)
	{
		Scene::AssignPhysicsBody(entity.GetUUID());
		Scene::AddCollider(entity.GetComponent<IDComponent>().m_ID, m_Shape);
	}

	void CapsuleCollider::OnComponentAdded(Entity& entity)
	{
		Scene::AssignPhysicsBody(entity.GetUUID());
		Scene::AddCollider(entity.GetComponent<IDComponent>().m_ID, m_Shape);
	}

	void MeshCollider::OnComponentAdded(Entity& entity)
	{
		Scene::AssignPhysicsBody(entity.GetUUID());
		Scene::AddCollider(entity.GetComponent<IDComponent>().m_ID, m_Shape);
	}

	void Rigidbody::OnComponentRemoved(Entity& entity)
	{
		Scene::RemoveRigidBody(entity.GetUUID());
	}

	void BoxCollider::OnComponentRemoved(Entity& entity)
	{
		Scene::RemoveCollider(entity.GetUUID());
	}

	void SphereCollider::OnComponentRemoved(Entity& entity)
	{
		Scene::RemoveCollider(entity.GetUUID());
	}

	void CapsuleCollider::OnComponentRemoved(Entity& entity)
	{
		Scene::RemoveCollider(entity.GetUUID());
	}

	void MeshCollider::OnComponentRemoved(Entity& entity)
	{
		Scene::RemoveCollider(entity.GetUUID());
	}
}