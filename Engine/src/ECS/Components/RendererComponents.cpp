#include "RendererComponents.hpp"
#include "ECS/Entity.hpp"

namespace Cresta
{
	void SpriteRenderer::OnComponentAdded(Entity& entity)
	{
		CRESTA_INFO("SpriteRenderer OnComponentAdded");
	}

	void MeshRenderer::OnComponentAdded(Entity& entity)
	{
		CRESTA_INFO("Mesh Renderer OnComponentAdded");
		int ID = static_cast<int>(entt::entity(entity));
		SetID(ID);
	}

	void SpriteRenderer::OnComponentRemoved(Entity& entity)
	{
		CRESTA_INFO("SpriteRenderer OnComponentRemoved");
	}

	void MeshRenderer::OnComponentRemoved(Entity& entity)
	{
		CRESTA_INFO("Mesh Renderer OnComponentRemoved");
	}
}