#pragma once
#include "Crestaph.hpp"
#include "PhysicsUtils.hpp"
#include "entt/entt.hpp"
#include "Scene/UUID.hpp"
#include <Jolt/Physics/StateRecorder.h>
#include <Jolt/Physics/StateRecorderImpl.h>

// Member variable to store the initial state
using namespace JPH;
using namespace JPH::literals;

namespace Cresta
{
	enum class ColliderShape
	{
		BoxCollider,
		SphereCollider,
		CapsuleCollider
	};

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

		void ClearBodies();
		
		void MakeBodyStatic(const UUID& EntityID);

		void RemoveCollider(const UUID& EntityID);

		void AddRigidBody(const BodyID& ID);

		void CreateBody(const UUID& EntityID,BodyID& ID);
		void AddCollider(const BodyID& ID, ColliderShape shape);

		void GetBodyPosition(const UUID& EntityID, glm::vec3& position);
		void GetBodyRotation(const UUID& EntityID, glm::quat& rotation);
		void SetBodyPosition(const UUID& EntityID, const glm::vec3& position);
		void SetBodyRotation(const UUID& EntityID, const glm::quat& rotation);

		void GetBodyPosition(const BodyID& ID, glm::vec3& position);
		void GetBodyRotation(const BodyID& ID, glm::quat& rotation);
		void SetBodyPosition(const BodyID& ID, const glm::vec3& position);
		void SetBodyRotation(const BodyID& ID, const glm::quat& rotation);

		void Step();
		void Start();
		void Stop();

	public:
		RVec3 position;
		Vec3 velocity;

	private:
		void Init();

	private:
		// Constants for physics simulation
		static constexpr uint cm_NumBodies = 10240;
		static constexpr uint cm_NumBodyMutexes = 0; // Auto-detect
		static constexpr uint cm_MaxBodyPairs = 65536;
		static constexpr uint cm_MaxContactConstraints = 20480;

		const RVec3 cm_Position = RVec3(0.0_r, 0.0_r, 0.0_r);
		const RVec3 cm_HalfExtents = RVec3(1.0f, 1.0f, 1.0f);

		const Quat cm_IdentityRotation = Quat::sIdentity();

		const float cm_CapsuleHalfHeight = 1.0f; // Half the distance between the hemispherical ends
		const float cm_Radius = 0.5f;     // Radius of the capsule
		const float cm_DeltaTime = 1.0f / 60.0f;

		// Jolt components

		StateRecorderImpl m_InitialStateRecorder;

		Scope<JobSystem> m_JobSystem;
		Scope<TempAllocator> m_TempAllocator;
		Scope<PhysicsSystem> m_PhysicsSystem;

		BodyInterface* m_BodyInterface;

		// Broadphase filters
		BPLayerInterfaceImpl m_BroadPhaseLayerInterface;
		ObjectLayerPairFilterImpl m_ObjectVsObjectLayerFilter;
		ObjectVsBroadPhaseLayerFilterImpl m_ObjectVsBroadPhaseLayerFilter;

		// Entity-to-Body map
		std::unordered_map<UUID, JPH::BodyID> m_EntityToBody;

		//Scope<DebugRenderer> m_DebugRenderer;
	};
}