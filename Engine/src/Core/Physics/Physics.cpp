#include "Physics.hpp"
#include "Physics.hpp"

namespace Cresta
{
	void Physics::TraceImpl(const char* inFMT, ...)
	{
		va_list list;
		va_start(list, inFMT);
		char buffer[1024];
		vsnprintf(buffer, sizeof(buffer), inFMT, list);
		va_end(list);
		std::cout << buffer << std::endl;
	}

#ifdef JPH_ENABLE_ASSERTS

	bool Physics::AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, uint inLine)
	{
		// Print to the TTY
		std::cout << inFile << ":" << inLine << ": (" << inExpression << ") " << (inMessage != nullptr ? inMessage : "") << std::endl;

		// Breakpoint
		return true;
	}
#endif // JPH_ENABLE_ASSERTS

	Physics::Physics()
	{

		Init();
	}

	Physics::~Physics()
	{
		ClearBodies();
		delete Factory::sInstance;
		Factory::sInstance = nullptr;
		UnregisterTypes();
	}

	void Physics::Init()
	{
		RegisterDefaultAllocator();
		Factory::sInstance = new Factory();
		RegisterTypes();

		m_JobSystem = CreateScope<JobSystemThreadPool>(cMaxPhysicsJobs, cMaxPhysicsBarriers, thread::hardware_concurrency() - 1);
		m_TempAllocator = CreateScope<TempAllocatorImpl>(32 * 1024 * 1024);
		m_PhysicsSystem = CreateScope<PhysicsSystem>();

		m_BodyInterface = &m_PhysicsSystem->GetBodyInterface();
		
		m_PhysicsSystem->Init(
			cm_NumBodies, 
			cm_NumBodyMutexes, 
			cm_MaxBodyPairs, 
			cm_MaxContactConstraints,
			m_BroadPhaseLayerInterface, 
			m_ObjectVsBroadPhaseLayerFilter, 
			m_ObjectVsObjectLayerFilter);

		//m_DebugRenderer = CreateScope<DebugRenderer>();
	}

	void Physics::MakeBodyStatic(const UUID& EntityID)
	{
		m_BodyInterface->SetMotionType(m_EntityToBody[EntityID], EMotionType::Static, EActivation::Activate);
		ObjectLayer Layer = m_BodyInterface->GetObjectLayer(m_EntityToBody[EntityID]);

		if (Layer == Layers::Colliders)
		{
			return;
		}
		else if (Layer == Layers::MOVING)
		{
			m_BodyInterface->SetObjectLayer(m_EntityToBody[EntityID], Layers::NON_MOVING);
		}
	}

	void Physics::RemoveCollider(const UUID& EntityID)
	{
		m_BodyInterface->SetObjectLayer(m_EntityToBody[EntityID], Layers::NoCollision);
	}

	void Physics::ClearBodies()
	{ 
		for (auto& entity : m_EntityToBody)
		{
			m_BodyInterface->RemoveBody(entity.second);
		}
		m_EntityToBody.clear();
	}

	void Physics::AddRigidBody(const BodyID& ID)
	{
		m_BodyInterface->SetMotionType(ID,EMotionType::Dynamic, EActivation::Activate);
		ObjectLayer Layer = m_BodyInterface->GetObjectLayer(ID);

		if (Layer == Layers::Colliders)
		{
			return;
		}
		
		m_BodyInterface->SetObjectLayer(ID,Layers::MOVING);
	}

	void Physics::CreateBody(const UUID& EntityID, BodyID& ID)
	{
		BodyCreationSettings settings(new SphereShape(1.0f),
			RVec3(0, 0, 0),
			Quat::sIdentity(),
			EMotionType::Static, Layers::NoCollision);

		settings.mAllowDynamicOrKinematic = true;

		Body* body = m_BodyInterface->CreateBody(settings);
		body->SetCollideKinematicVsNonDynamic(true);

		m_EntityToBody[EntityID] = m_BodyInterface->CreateAndAddBody(settings,EActivation::Activate);
		ID = m_EntityToBody[EntityID];
	}

	void Physics::AddCollider(const BodyID& ID, ColliderShape shape)
	{
		m_BodyInterface->SetObjectLayer(ID, Layers::Colliders);
	
		BodyLockWrite lock(m_PhysicsSystem->GetBodyLockInterface(), ID);
		if (lock.Succeeded()) 
		{
			Body& body = lock.GetBody();
			switch (shape)
			{
			case ColliderShape::BoxCollider:
					body.SetShapeInternal(new BoxShape(cm_HalfExtents), true);
				break;
			case ColliderShape::SphereCollider:
					body.SetShapeInternal(new SphereShape(cm_Radius), true);
				break;
			case ColliderShape::CapsuleCollider:
					body.SetShapeInternal(new CapsuleShape(cm_CapsuleHalfHeight, cm_Radius), true);
				break;
			}
			lock.ReleaseLock();
		}
	}

	void Physics::GetBodyPosition(const UUID& EntityID, glm::vec3& position)
	{
		RVec3 bodyPosition = m_BodyInterface->GetCenterOfMassPosition(m_EntityToBody[EntityID]);
		position.x = bodyPosition.GetX();
		position.y = bodyPosition.GetY();
		position.z = bodyPosition.GetZ();
	}

	void Physics::SetBodyPosition(const UUID& EntityID, const glm::vec3& position)
	{
		m_BodyInterface->SetPosition(m_EntityToBody[EntityID], { position.x, position.y, position.z }, EActivation::Activate);
	}

	void Physics::SetBodyScale(const UUID& EntityID, const glm::vec3& scale)
	{
		// Retrieve the body ID from our entity-to-body map.
		BodyID body_id = m_EntityToBody[EntityID];

		// Lock the body for writing.
		BodyLockWrite lock(m_PhysicsSystem->GetBodyLockInterface(), body_id);
		if (!lock.Succeeded())
			return;

		Body& body = lock.GetBody();
		const Shape* currentShape = body.GetShape();
		if (currentShape == nullptr)
		{
			lock.ReleaseLock();
			return;
		}

		Shape* newShape = nullptr;

		// Check if the shape is a BoxShape.
		if (const BoxShape* box = dynamic_cast<const BoxShape*>(currentShape))
		{
			// Get current half extents and apply non-uniform scaling.
			RVec3 currentHalfExtents = box->GetHalfExtent();
			RVec3 newHalfExtents(
				currentHalfExtents.GetX() * scale.x,
				currentHalfExtents.GetY() * scale.y,
				currentHalfExtents.GetZ() * scale.z
			);
			newShape = new BoxShape(newHalfExtents);
		}
		// Check if the shape is a SphereShape.
		else if (const SphereShape* sphere = dynamic_cast<const SphereShape*>(currentShape))
		{
			// For a sphere, uniform scaling is assumed.
			float newRadius = sphere->GetRadius() * scale.x; // or choose an average scale factor
			newShape = new SphereShape(newRadius);
		}
		// Check if the shape is a CapsuleShape.
		else if (const CapsuleShape* capsule = dynamic_cast<const CapsuleShape*>(currentShape))
		{
			// Assume that the capsule's radius scales uniformly (using scale.x) and
			// its half-height scales with scale.y (this choice may vary based on your needs).
			float newRadius = capsule->GetRadius() * scale.x;
			float newHalfHeight = capsule->GetHalfHeightOfCylinder() * scale.y;
			newShape = new CapsuleShape(newHalfHeight, newRadius);
		}

		// If a new shape was created, assign it to the body.
		if (newShape != nullptr)
		{
			// The second parameter 'true' indicates that the old shape should be deleted.
			body.SetShapeInternal(newShape, true);
		}

		lock.ReleaseLock();
	}


	void Physics::GetBodyRotation(const UUID& EntityID, glm::quat& rotation)
	{
		Quat bodyRotation = m_BodyInterface->GetRotation(m_EntityToBody[EntityID]);
		rotation.x = bodyRotation.GetX();
		rotation.y = bodyRotation.GetY();
		rotation.z = bodyRotation.GetZ();
		rotation.w = bodyRotation.GetW();
	}

	void Physics::SetBodyRotation(const UUID& EntityID, const glm::quat& rotation)
	{
		m_BodyInterface->SetRotation(m_EntityToBody[EntityID], { rotation.x, rotation.y, rotation.z , rotation.w}, EActivation::Activate);
	}

	void Physics::GetBodyPosition(const BodyID& ID, glm::vec3& position)
	{
		RVec3 bodyPosition = m_BodyInterface->GetCenterOfMassPosition(ID);
		position.x = bodyPosition.GetX();
		position.y = bodyPosition.GetY();
		position.z = bodyPosition.GetZ();
	}

	void Physics::SetBodyPosition(const BodyID& ID, const glm::vec3& position)
	{
		m_BodyInterface->SetPosition(ID, { position.x, position.y, position.z }, EActivation::Activate);
	}

	void Physics::GetBodyRotation(const BodyID& ID, glm::quat& rotation)
	{
		Quat bodyRotation = m_BodyInterface->GetRotation(ID);
		rotation.x = bodyRotation.GetX();
		rotation.y = bodyRotation.GetY();
		rotation.z = bodyRotation.GetZ();
		rotation.w = bodyRotation.GetW();
	}

	void Physics::SetBodyRotation(const BodyID& ID, const glm::quat& rotation)
	{
		m_BodyInterface->SetRotation(ID, { rotation.x, rotation.y, rotation.z , rotation.w }, EActivation::Activate);
	}

	void Physics::Step()
	{	
		const int cCollisionSteps = 1;
		m_PhysicsSystem->Update(cm_DeltaTime, cCollisionSteps, m_TempAllocator.get(), m_JobSystem.get());
	}

	void Physics::Start()
	{
		m_InitialStateRecorder.Clear();
		m_PhysicsSystem->SaveState(m_InitialStateRecorder);
	}

	void Cresta::Physics::Stop()
	{
		m_PhysicsSystem->RestoreState(m_InitialStateRecorder);
	}
}