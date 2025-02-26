#include "RendererComponents.hpp"
#include "RendererComponents.hpp"
#include "RendererComponents.hpp"
#include "ECS/Entity.hpp"

namespace Cresta
{
	void SpriteRenderer::OnComponentAdded()
	{
		CRESTA_INFO("SpriteRenderer OnComponentAdded");
	}

	MeshRenderer::MeshRenderer(Entity* entity, const std::string& path, int id) : ComponentTemplate(entity), m_Path(path)
	{
		m_ID = static_cast<int>(entt::entity(*entity));
		PathChanged();
	}

	void MeshRenderer::OnUpdate()
	{
		Draw(p_Entity->GetComponent<Transform>().GetTransform());
	}

	void MeshRenderer::OnComponentAdded()
	{
		CRESTA_INFO("Mesh Renderer OnComponentAdded");
	}

	void SpriteRenderer::OnComponentRemoved()
	{
		CRESTA_INFO("SpriteRenderer OnComponentRemoved");
	}

	void MeshRenderer::OnComponentRemoved()
	{
		CRESTA_INFO("Mesh Renderer OnComponentRemoved");
	}
}