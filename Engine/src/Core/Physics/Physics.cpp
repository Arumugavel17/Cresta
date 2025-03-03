#include "Physics.hpp"
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

namespace Cresta
{
	Scope<PhysicsController> Physics::sm_PhysicsController = CreateScope<PhysicsController>();

	void Physics::ClearBodies()
	{
		sm_PhysicsController->ClearBodies();
	}

	void Physics::MakeBodyStatic(const UUID& EntityID)
	{
		sm_PhysicsController->MakeBodyStatic(EntityID);
	}

	void Physics::CreateBody(const UUID& EntityID)
	{
		sm_PhysicsController->CreateBody(EntityID);
	}

	void Physics::AddRigidBody(const UUID& EntityID)
	{
		sm_PhysicsController->AddRigidBody(EntityID);
	}

	void Physics::RemoveCollider(const UUID& EntityID)
	{
		sm_PhysicsController->RemoveCollider(EntityID);
	}

	void Physics::AddCollider(const UUID& EntityID, const ColliderShape& shape)
	{
		sm_PhysicsController->AddCollider(EntityID, shape);
	}

	void Physics::SetBodyPosition(const UUID& EntityID, const glm::vec3& position)
	{
		sm_PhysicsController->SetBodyPosition(EntityID, position);
	}

	void Physics::SetBodyShapeOffset(const UUID& EntityID, const glm::vec3& scale)
	{
		sm_PhysicsController->SetBodyShapeOffset(EntityID, scale);
	}

	void Physics::SetBodyShapeScale(const UUID& EntityID, const glm::vec3& scale)
	{
		sm_PhysicsController->SetBodyShapeScale(EntityID, scale);
	}

	glm::quat Physics::GetBodyRotation(const UUID& EntityID)
	{
		return sm_PhysicsController->GetBodyRotation(EntityID);
	}

	void Physics::SetBodyRotation(const UUID& EntityID, const glm::quat& rotation)
	{
		sm_PhysicsController->SetBodyRotation(EntityID, rotation);
	}

	glm::vec3 Physics::GetBodyPosition(const UUID& EntityID)
	{
		return sm_PhysicsController->GetBodyPosition(EntityID);
	}

	void Physics::Step()
	{	
		sm_PhysicsController->Step();
	}

	void Physics::Start()
	{
		sm_PhysicsController->Start();
	}

	void Cresta::Physics::Stop()
	{
		sm_PhysicsController->Stop();
	}
}