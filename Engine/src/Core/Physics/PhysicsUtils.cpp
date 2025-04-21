#include "PhysicsUtils.hpp"
#include "PhysicsController.hpp"

namespace Cresta
{
	void ColliderContactListener::DispatchEvent(CollisionEventType EventType, const Body& inBody1, const Body& inBody2)
	{
		CollisionEvent event(EventType,inBody1,inBody2);
		PhysicsController::sm_Instance->OnCollision(event);
	}
}