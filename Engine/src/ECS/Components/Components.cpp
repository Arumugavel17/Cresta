#include "ECS/Components/Components.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Scene/Scene.hpp"

namespace Cresta
{
	void IDComponent::OnComponentAdded(Entity& entity)
	{
		entity.GetComponent<IDComponent>().m_ID = UUID();
		CRESTA_INFO("IDComponent OnComponentAdded");
	}

	void TagComponent::OnComponentAdded(Entity& entity)
	{
		CRESTA_INFO("TagComponent OnComponentAdded");
	}

	void Transform::OnComponentAdded(Entity& entity)
	{
		CRESTA_INFO("Transform OnComponentAdded");
	}

	void CameraComponent::OnComponentAdded(Entity& entity)
	{
		CRESTA_INFO("CameraComponent OnComponentAdded");
	}

	//Removed////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void IDComponent::OnComponentRemoved(Entity& entity)
	{
		CRESTA_INFO("IDComponent OnComponentRemoved");
	}

	void TagComponent::OnComponentRemoved(Entity& entity)
	{
		CRESTA_INFO("TagComponent OnComponentRemoved");
	}

	void Transform::OnComponentRemoved(Entity& entity)
	{
		CRESTA_INFO("Transform OnComponentRemoved");
	}

	void CameraComponent::OnComponentRemoved(Entity& entity)
	{
		CRESTA_INFO("CameraComponent OnComponentRemoved");
	}
}