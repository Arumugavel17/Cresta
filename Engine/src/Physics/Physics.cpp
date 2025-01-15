#include "Physics/Physics.hpp"

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

		RegisterDefaultAllocator();
		Trace = TraceImpl;
		JPH_IF_ENABLE_ASSERTS(AssertFailed = AssertFailedImpl;)
		Factory::sInstance = new Factory();
		RegisterTypes(); 
		Init(); 
	}

	Physics::~Physics()
	{
		m_BodyInterface->RemoveBody(m_SphereId);
		m_BodyInterface->DestroyBody(m_SphereId);
		m_BodyInterface->RemoveBody(m_Floor->GetID());
		m_BodyInterface->DestroyBody(m_Floor->GetID());

		UnregisterTypes();

		delete Factory::sInstance;
		Factory::sInstance = nullptr;
	}
		
	void Physics::Init()
	{
		m_TempAllocator = CreateRef<TempAllocatorImpl>(10 * 1024 * 1024);
		m_JobSystem = CreateRef<JobSystemThreadPool>(cMaxPhysicsJobs, cMaxPhysicsBarriers, thread::hardware_concurrency() - 1); 
		m_BroadPhaseLayerInterface = CreateRef<BPLayerInterfaceImpl>();
		m_ObjectVsBroadphaseLayerFilter = CreateRef<ObjectVsBroadPhaseLayerFilterImpl>();
		m_ObjectVsObjectLayerFilter = CreateRef<ObjectLayerPairFilterImpl>();
		m_PhysicsSystem = CreateRef<PhysicsSystem>();
		m_BodyActivationListener = CreateRef<MyBodyActivationListener>();
		m_ContactListener = CreateRef<MyContactListener>();

		m_PhysicsSystem->Init(c_MaxBodies, c_NumBodyMutexes, c_MaxBodyPairs, c_MaxContactConstraints, *m_BroadPhaseLayerInterface, *m_ObjectVsBroadphaseLayerFilter, *m_ObjectVsObjectLayerFilter);
		m_PhysicsSystem->SetBodyActivationListener(m_BodyActivationListener.get());
		m_PhysicsSystem->SetContactListener(m_ContactListener.get());

		m_BodyInterface = &m_PhysicsSystem->GetBodyInterface();

		m_FloorShapeSettings = CreateRef<BoxShapeSettings>(Vec3(100.0f, 1.0f, 100.0f));
		m_FloorShapeSettings->SetEmbedded();

		m_FloorShapeResult = m_FloorShapeSettings->Create();
		m_FloorShape = m_FloorShapeResult.Get();

		m_FloorSettings = CreateRef<BodyCreationSettings>(m_FloorShape, RVec3(0.0_r, -1.0_r, 0.0_r), Quat::sIdentity(), EMotionType::Static, Layers::NON_MOVING);
		m_Floor = m_BodyInterface->CreateBody(*m_FloorSettings); 

		m_BodyInterface->AddBody(m_Floor->GetID(), EActivation::DontActivate);
		m_SphereSettings = CreateRef<BodyCreationSettings>(new SphereShape(0.5f), RVec3(0.0_r, 2.0_r, 0.0_r), Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING);
		m_SphereId = m_BodyInterface->CreateAndAddBody(*m_SphereSettings, EActivation::Activate);
	}

	void Physics::Run()
	{
		m_PhysicsSystem->OptimizeBroadPhase();
		while (m_BodyInterface->IsActive(m_SphereId)) 
		{
			++m_Step;
			RVec3 position = m_BodyInterface->GetCenterOfMassPosition(m_SphereId);
			Vec3 velocity = m_BodyInterface->GetLinearVelocity(m_SphereId);
			std::cout << "Step " << m_Step << ": Position = (" << position.GetX() << ", " << position.GetY() << ", " << position.GetZ() << "), Velocity = (" << velocity.GetX() << ", " << velocity.GetY() << ", " << velocity.GetZ() << ")" << std::endl;

			const int cCollisionSteps = 1;

			m_PhysicsSystem->Update(c_DeltaTime, cCollisionSteps, m_TempAllocator.get(), m_JobSystem.get());
		}
	}
}
