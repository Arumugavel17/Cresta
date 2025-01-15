#include "Crestaph.hpp"
#include "Physics/PhysicsUtils.hpp"

using namespace JPH;
using namespace JPH::literals;

namespace Cresta
{
	class Physics
	{
	public:
		static void TraceImpl(const char* inFMT, ...);

#ifdef JPH_ENABLE_ASSERTS
		// Callback for asserts, connect this to your own assert handler if you have one
		static bool AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, uint inLine);
#endif // JPH_ENABLE_ASSERTS
		
		Physics();
		~Physics();

		void Run();
	private:
		void Init();
		const uint c_MaxBodies = 1024;
		const uint c_NumBodyMutexes = 0;
		const uint c_MaxBodyPairs = 1024;
		const uint c_MaxContactConstraints = 1024;
		const float c_DeltaTime = 1.0f / 60.0f;
		uint m_Step = 0;

		Ref<TempAllocatorImpl> m_TempAllocator;
		Ref<JobSystemThreadPool> m_JobSystem;
		Ref<BPLayerInterfaceImpl> m_BroadPhaseLayerInterface;
		Ref<ObjectVsBroadPhaseLayerFilterImpl> m_ObjectVsBroadphaseLayerFilter;
		Ref<ObjectLayerPairFilterImpl> m_ObjectVsObjectLayerFilter;
		Ref<PhysicsSystem> m_PhysicsSystem;
		Ref<MyBodyActivationListener> m_BodyActivationListener;
		Ref<MyContactListener> m_ContactListener;
		Ref<BoxShapeSettings> m_FloorShapeSettings;
		Ref<BodyCreationSettings> m_FloorSettings;
		Ref<BodyCreationSettings> m_SphereSettings;

		BodyInterface* m_BodyInterface;
		ShapeSettings::ShapeResult m_FloorShapeResult; 
		ShapeRefC m_FloorShape;
		Body* m_Floor;
		BodyID m_SphereId;
	};
}