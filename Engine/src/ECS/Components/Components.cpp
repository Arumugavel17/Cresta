#include "Components.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Scene/Scene.hpp"

namespace Cresta
{
	void IDComponent::OnComponentAdded(Entity& entity)
	{
		entity.GetComponent<IDComponent>().m_ID = UUID();
	}

	void TagComponent::OnComponentAdded(Entity& entity)
	{
	}

	void Transform::OnComponentAdded(Entity& entity)
	{
	}

	void CameraComponent::OnComponentAdded(Entity& entity)
	{
	}

	//Removed////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void IDComponent::OnComponentRemoved(Entity& entity)
	{
	}

	void TagComponent::OnComponentRemoved(Entity& entity)
	{
	}

	void Transform::OnComponentRemoved(Entity& entity)
	{
	}

	void CameraComponent::OnComponentRemoved(Entity& entity)
	{
	}
}