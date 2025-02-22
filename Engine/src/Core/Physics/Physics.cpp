#include "Physics.hpp"
#include "Physics.hpp"
#include <Jolt/Physics/Collision/Shape/ScaledShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

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

	void Physics::AddRigidBody(const UUID& EntityID)
	{
		m_BodyInterface->SetMotionType(m_EntityToBody[EntityID],EMotionType::Dynamic, EActivation::Activate);
		ObjectLayer Layer = m_BodyInterface->GetObjectLayer(m_EntityToBody[EntityID]);

		if (Layer == Layers::Colliders)
		{
			return;
		}
		
		m_BodyInterface->SetObjectLayer(m_EntityToBody[EntityID],Layers::MOVING);
	}

	void Physics::CreateBody(const UUID& EntityID)
	{
		if (m_EntityToBody.find(EntityID) != m_EntityToBody.end())
		{
			return;
		}
		RefConst<Shape> scaled_sphere_shape = /* new ScaledShape( */new SphereShape(1.0f)/*, {1.0f,1.0f,1.0f})*/;
		BodyCreationSettings settings(scaled_sphere_shape,
			RVec3(0, 0, 0),
			Quat::sIdentity(),
			EMotionType::Static, Layers::NoCollision);

		settings.mAllowDynamicOrKinematic = true;
		
		m_EntityToBody[EntityID] = m_BodyInterface->CreateAndAddBody(settings,EActivation::Activate);
	}

	void Physics::AddCollider(const UUID& EntityID, const ColliderShape& shape)
	{
		m_BodyInterface->SetObjectLayer(m_EntityToBody[EntityID], Layers::Colliders);
	
		BodyLockWrite lock(m_PhysicsSystem->GetBodyLockInterface(), m_EntityToBody[EntityID]);
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
	void Physics::SetBodyPosition(const UUID& EntityID, const glm::vec3& position)
	{
		m_BodyInterface->SetPosition(m_EntityToBody[EntityID], { position.x, position.y, position.z }, EActivation::Activate);
	}

	void Physics::SetBodyScale(const UUID& EntityID, const glm::vec3& scale)
	{
		BodyID body_id = m_EntityToBody[EntityID];
		BodyLockWrite lock(m_PhysicsSystem->GetBodyLockInterface(), body_id);
		Body* body;
		if (lock.Succeeded())
		{
			Vec3 Scale({ scale.x,scale.y,scale.z });
			body = &lock.GetBody();
			Vec3 BodyScale = body->GetShape()->GetLocalBounds().GetExtent();
			if(Scale == BodyScale)
			{
				lock.ReleaseLock();
				return;
			}
			BodyScale = Scale / BodyScale;
			Shape::ShapeResult new_shape = body->GetShape()->ScaleShape(BodyScale);
			if (new_shape.IsValid())
			{
				m_PhysicsSystem->GetBodyInterfaceNoLock().SetShape(body->GetID(), new_shape.Get(), true, EActivation::Activate);
			}
			lock.ReleaseLock();
			AABox Box = body->GetShape()->GetLocalBounds().Scaled(Vec3::sReplicate(10.0f));
			
			m_BodyInterface->ActivateBodiesInAABox(
				Box,
				m_PhysicsSystem->GetDefaultBroadPhaseLayerFilter(body->GetObjectLayer()),
				m_PhysicsSystem->GetDefaultLayerFilter(body->GetObjectLayer())
				);
		}
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

	void Physics::GetBodyPosition(const UUID& EntityID, glm::vec3& position)
	{
		RVec3 bodyPosition = m_BodyInterface->GetCenterOfMassPosition(m_EntityToBody[EntityID]);
		position.x = bodyPosition.GetX();
		position.y = bodyPosition.GetY();
		position.z = bodyPosition.GetZ();
	}

	void Physics::Step()
	{	
		const int cCollisionSteps = 1;
		m_PhysicsSystem->OptimizeBroadPhase();
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