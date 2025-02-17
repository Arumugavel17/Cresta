#include "ECS/Components/Components.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Scene/Scene.hpp"

namespace Cresta
{
	void IDComponent::OnComponentAdded(Entity& entity)
	{
		entity.GetComponent<IDComponent>().m_ID = UUID();
		CRESTA_INFO("IDComponent OnComponentAdded");
	}

	void TagComponent::OnComponentAdded(Entity& entity)
	{
		CRESTA_INFO("TagComponent OnComponentAdded");
	}

	void Transform::OnComponentAdded(Entity& entity)
	{
		CRESTA_INFO("Transform OnComponentAdded");
	}

	void SpriteRenderer::OnComponentAdded(Entity& entity)
	{
		CRESTA_INFO("SpriteRenderer OnComponentAdded");
	}

	void MeshRenderer::OnComponentAdded(Entity& entity)
	{
		CRESTA_INFO("Mesh Renderer OnComponentAdded");
		int ID = static_cast<int>(entt::entity(entity));
		SetID(ID);
	}

	void CameraComponent::OnComponentAdded(Entity& entity)
	{
		CRESTA_INFO("CameraComponent OnComponentAdded");
	}

	void PhysicsComponent::OnComponentAdded(Entity& entity)
	{
		CRESTA_INFO("PhysicsComponent OnComponentAdded");
		Scene::m_Physics->CreateBody(entity.GetUUID(), BodyID);
	}

	void Rigidbody::OnComponentAdded(Entity& entity)
	{
		if (!entity.HasComponent<PhysicsComponent>())
		{
			entity.AddComponent<PhysicsComponent>();
		}
		Scene::m_Physics->AddRigidBody(entity.GetComponent<PhysicsComponent>().BodyID);

		CRESTA_INFO("Rigidbody OnComponentAdded");
	}

	void BoxCollider::OnComponentAdded(Entity& entity)
	{
		if (!entity.HasComponent<PhysicsComponent>())
		{
			entity.AddComponent<PhysicsComponent>();
		}
		Scene::m_Physics->AddCollider(entity.GetComponent<PhysicsComponent>().BodyID,m_Shape);

		CRESTA_INFO("BoxCollider OnComponentAdded");
	}

	void SphereCollider::OnComponentAdded(Entity& entity)
	{
		if (!entity.HasComponent<PhysicsComponent>())
		{
			entity.AddComponent<PhysicsComponent>();
		}
		Scene::m_Physics->AddCollider(entity.GetComponent<PhysicsComponent>().BodyID, m_Shape);

		CRESTA_INFO("SphereCollider OnComponentAdded");
	}

	void CapsuleCollider::OnComponentAdded(Entity& entity)
	{
		if (!entity.HasComponent<PhysicsComponent>())
		{
			entity.AddComponent<PhysicsComponent>();
		}
		Scene::m_Physics->AddCollider(entity.GetComponent<PhysicsComponent>().BodyID, m_Shape);

		CRESTA_INFO("CapsuleCollider OnComponentAdded");
	}

	void MeshCollider::OnComponentAdded(Entity& entity)
	{
		if (!entity.HasComponent<PhysicsComponent>())
		{
			entity.AddComponent<PhysicsComponent>();
		}
		Scene::m_Physics->AddCollider(entity.GetComponent<PhysicsComponent>().BodyID, m_Shape);

		CRESTA_INFO("MeshCollider OnComponentAdded");
	}

	void ClassComponent::OnComponentAdded(Entity& entity)
	{
		CRESTA_INFO("ClassComponent OnComponentAdded");
	}
}