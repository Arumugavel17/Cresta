#include "RendererComponents.hpp"
#include "Renderer/PrimitiveMeshes.hpp"
#include "ECS/Entity.hpp"
#include "Core/Time.hpp"

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

	void MeshRenderer::OnRender()
	{
		CRESTA_PROFILE_FUNCTION();

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

	void AnimatorComponent::OnComponentAdded()
	{
		MeshRenderer* meshRenderer;
		CRESTA_INFO("Animator Component OnComponentAdded");
		if (!p_Entity->HasComponent<MeshRenderer>())
		{
			meshRenderer = &p_Entity->AddComponent<MeshRenderer>();
		}
		else
		{
			meshRenderer = &p_Entity->GetComponent<MeshRenderer>();
		}

		if (meshRenderer)
		{
			m_Model = meshRenderer->GetModel();
		}
	}

	void AnimatorComponent::OnUpdate()
	{
		UpdateAnimation();
	}

	void AnimatorComponent::UpdateAnimation()
	{
		m_Animator.UpdateAnimation(Time::DeltaTime());
		std::vector<glm::mat4>& Transform = m_Animator.GetFinalBoneMatrices();
		for (int i = 0;i < Transform.size();i++)
		{
			//m_Model->SetShaderUniform(Transform[i], "finalBonesMatrices[" + std::to_string(i) + "]");
		}
	}

	void AnimatorComponent::OnComponentRemoved()
	{
		CRESTA_INFO("Animator Component OnComponentAdded");
	}
}