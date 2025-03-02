#include "PhysicsComponents.hpp"
#include "Renderer/PrimitiveMeshes.hpp"
#include "ECS/Entity.hpp"
#include "Renderer/Renderer.hpp"

namespace Cresta
{
	void Rigidbody::OnStart()
	{

	}

	void Rigidbody::OnFixedUpdate()
	{
		glm::quat Rotation;
		glm::vec3 Translation(0.0f);
		Physics::GetBodyPosition(p_Entity->GetUUID(), Translation);
		Physics::GetBodyRotation(p_Entity->GetUUID(), Rotation);

		p_Entity->GetComponent<Transform>().SetTranslation(Translation);
		p_Entity->GetComponent<Transform>().SetRotation(Rotation);
	}
	void Rigidbody::OnComponentAdded()
	{
		Scene::AddRigidBody(p_Entity->GetComponent<IDComponent>().GetUUID());
	}

	BoxCollider::BoxCollider(Entity* entity) : Collider(entity, ColliderShape::BoxCollider)
	{
		Transform& transform = p_Entity->GetComponent<Transform>();
		transform.OnValidate.subscribe(typeid(BoxCollider).name(), [entity = p_Entity]()
			{
				BoxCollider::Revaluate(entity);
			});

		m_Rotation = transform.GetRotation(); 
		m_Center = transform.GetTranslation();
		m_Size = transform.GetScale();

		OffestLocalRotation = transform.GetRotation();
		OffestLocalCenter = transform.GetTranslation();
		OffestLocalSize = transform.GetScale();
	}

	void BoxCollider::OnComponentAdded()
	{
		Scene::AddCollider(p_Entity->GetComponent<IDComponent>().GetUUID(), m_Shape);
	}

	void BoxCollider::Revaluate(Entity* entity)
	{
		BoxCollider& collider = entity->GetComponent<BoxCollider>();

		auto& transform = entity->GetComponent<Transform>();
		// Apply transformation to the stored local values
		glm::mat4 transformMatrix = transform.GetTransform();

		glm::quat parentRotation = glm::quat(transform.GetRotation()); // Parent rotation
		glm::quat childRotation = glm::quat(collider.OffestLocalRotation); // Local rotation
		collider.m_Center = glm::vec3(transformMatrix * glm::vec4(collider.OffestLocalCenter, 1.0f));
		collider.m_Rotation = glm::eulerAngles(parentRotation * childRotation);
		collider.m_Size = transform.GetScale() * collider.OffestLocalSize; // Scale transformation

		Physics::SetBodyScale(entity->GetUUID(), collider.m_Size);
	}

	void BoxCollider::OnStart()
	{
		Physics::SetBodyPosition(p_Entity->GetUUID(),m_Center);
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