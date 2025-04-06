#pragma once
#include "PhysicsController.hpp"

// Member variable to store the initial state
using namespace JPH;
using namespace JPH::literals;

namespace Cresta
{
	class Scene;

	class Physics
	{
	public:
		static void ClearBodies();
		static void MakeBodyStatic(const UUID& EntityID);
		static void CreateBody(const UUID& EntityID);

		static void AddRigidBody(const UUID& EntityID);
		static void AddCollider(const UUID& EntityID,const ColliderShape& shape);
		static void SetColliderTrigger(const UUID& EntityID,bool IsTrigger);
		static void RemoveCollider(const UUID& EntityID);
		
		static glm::vec3 GetBodyPosition(const UUID& EntityID);
		static glm::quat GetBodyRotation(const UUID& EntityID);
		
		static void SetBodyPosition(const UUID& EntityID, const glm::vec3& position);
		static void SetBodyRotation(const UUID& EntityID, const glm::quat& rotation);

		static void SetGravityFactor(const UUID& EntityID, float gravityfactor);

		static void SetBodyShapeOffset(const UUID& EntityID, const glm::vec3& scale);
		static void SetBodyRadius(const UUID& EntityID, const float radius);
		static void SetBodyShapeScale(const UUID& EntityID, const glm::vec3& scale);

	private:
		static void Step();
		static void Start();
		static void Stop();

	private:
		static Scope<PhysicsController> sm_PhysicsController;
		friend class Scene;
	};
}