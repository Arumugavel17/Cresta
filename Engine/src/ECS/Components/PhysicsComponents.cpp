#include "PhysicsComponents.hpp"
#include "Renderer/PrimitiveMeshes.hpp"
#include "ECS/Entity.hpp"
#include "Renderer/Renderer.hpp"

namespace Cresta
{
	void Rigidbody::OnStart()
	{
		Physics::SetBodyPosition(p_Entity->GetUUID(), p_Entity->GetComponent<Transform>().GetTranslation());
		Physics::SetBodyRotation(p_Entity->GetUUID(), p_Entity->GetComponent<Transform>().GetRotation());
	}

	void Rigidbody::OnFixedUpdate()
	{
		p_Entity->GetComponent<Transform>().SetTranslation(
			Physics::GetBodyPosition(p_Entity->GetUUID()
		));

		p_Entity->GetComponent<Transform>().SetRotation(
			Physics::GetBodyRotation(p_Entity->GetUUID()
		));
	}

	void Rigidbody::OnComponentAdded()
	{
		Scene::AddRigidBody(p_Entity->GetComponent<IDComponent>().GetUUID());
	}

	BoxCollider::BoxCollider(Entity* entity) : Collider(entity, ColliderShape::BoxCollider) {}

	void BoxCollider::OnComponentAdded()
	{
		Scene::AddCollider(p_Entity->GetComponent<IDComponent>().GetUUID(), m_Shape);

		m_Rotation = glm::vec3(0.0f);
		m_Center = glm::vec3(0.0f);
		m_Scale = glm::vec3(1.0f);

		Transform& transform = p_Entity->GetComponent<Transform>();
		transform.OnValidate.Subscribe(typeid(BoxCollider).name(), [entity = p_Entity]()
			{
				BoxCollider::Revaluate(entity);
			});

		OffestLocalCenter = glm::vec3(0.0f);
		OffestLocalRotation = transform.GetRotation();
		OffestLocalScale = transform.GetScale();

		Revaluate(p_Entity);
	}

	void BoxCollider::Revaluate(Entity* entity)
	{
		auto& collider = entity->GetComponent<BoxCollider>();
		auto& transform = entity->GetComponent<Transform>();

		// Apply transformation to the stored local values
		glm::mat4 transformMatrix = transform.GetTransform();

		glm::quat parentRotation = glm::quat(transform.GetRotation()); // Parent rotation
		glm::quat childRotation = glm::quat(collider.OffestLocalRotation); // Local rotation

		collider.m_Center = glm::vec3(transformMatrix * glm::vec4(collider.OffestLocalCenter, 1.0f));
		collider.m_Rotation = glm::eulerAngles(parentRotation * childRotation);
		collider.m_Scale = transform.GetScale() * collider.OffestLocalScale; // Scale transformation

		Physics::SetBodyPosition(entity->GetUUID(), collider.m_Center);
		Physics::SetBodyRotation(entity->GetUUID(), collider.m_Rotation);

		if (std::abs(collider.m_Scale.x) > 0.1f && std::abs(collider.m_Scale.y) > 0.1f && std::abs(collider.m_Scale.z) > 0.1f)
		{
			Physics::SetBodyShapeScale(entity->GetUUID(), collider.m_Scale);
		}
	}

	void BoxCollider::OnStart()
	{
		if (std::abs(m_Scale.x) > 0.1f && std::abs(m_Scale.y) > 0.1f && std::abs(m_Scale.z) > 0.1f)
		{
			Physics::SetBodyShapeScale(p_Entity->GetUUID(), m_Scale);
		}
	}

	void BoxCollider::OnUpdate()
	{
		Primitive::DrawCubeGuizmo(GetTransform());
	}
	
	void BoxCollider::OnFixedUpdate()
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