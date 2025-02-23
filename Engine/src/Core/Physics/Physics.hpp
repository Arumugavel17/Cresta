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
		static void RemoveCollider(const UUID& EntityID);

		static void GetBodyPosition(const UUID& EntityID, glm::vec3& position);
		static void GetBodyRotation(const UUID& EntityID, glm::quat& rotation);
		static void SetBodyPosition(const UUID& EntityID, const glm::vec3& position);
		static void SetBodyRotation(const UUID& EntityID, const glm::quat& rotation);
		static void SetBodyScale(const UUID& EntityID, const glm::vec3& scale);

	private:
		static void Step();
		static void Start();
		static void Stop();

	private:
		static Scope<PhysicsController> sm_PhysicsController;
		friend class Scene;
	};
}