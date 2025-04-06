#include "AnimationController.hpp"
#include "ECS/Components/RendererComponents.hpp"
#include "ECS/Entity.hpp"

namespace Cresta
{

	Ref<Shader> AnimatorComponent::sm_AnimationShader = nullptr;
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

			meshRenderer->SetShader(MeshRenderer::ShaderType::ANIMATION, sm_AnimationShader);
			meshRenderer->SetShaderType(MeshRenderer::ShaderType::ANIMATION);
			meshRendererComponent = meshRenderer;
		}
		if (meshRendererComponent)
		{
			sm_AnimationShader->Bind();
			for (int i = 0;i < 200;i++)
			{
				sm_AnimationShader->SetMat4("finalBonesMatrices[" + std::to_string(i) + "]", glm::mat4(1.0f));
			}
		}
	}

	void AnimatorComponent::OnStart()
	{
		if (meshRendererComponent)
		{
			sm_AnimationShader->Bind();
			for (int i = 0;i < 200;i++)
			{
				sm_AnimationShader->SetMat4("finalBonesMatrices[" + std::to_string(i) + "]", glm::mat4(1.0f));
			}
		}
		m_Animator.ResetTime();
		m_Animator.StartAnimation();
	}

	void AnimatorComponent::OnUpdate()
	{
		UpdateAnimation();
	}

	void AnimatorComponent::OnEnd()
	{
		if (meshRendererComponent)
		{
			sm_AnimationShader->Bind();
			for (int i = 0;i < 200;i++)
			{
				sm_AnimationShader->SetMat4("finalBonesMatrices[" + std::to_string(i) + "]", glm::mat4(1.0f));
			}
		}
		m_Animator.EndAnimation();
	}

	void AnimatorComponent::UpdateAnimation()
	{

		if (meshRendererComponent)
		{
			m_Animator.EndAnimation();
			m_Animator.StartAnimation();
			std::vector<glm::mat4>& Transform = m_Animator.GetFinalBoneMatrices();
			sm_AnimationShader->Bind();
			for (int i = 0;i < 200;i++)
			{
				sm_AnimationShader->SetMat4("finalBonesMatrices[" + std::to_string(i) + "]", Transform[i]);
			}
		}
	}

	void AnimatorComponent::PathChanged()
	{
		if (meshRendererComponent)
		{
			sm_AnimationShader->Bind();
			for (int i = 0;i < 200;i++)
			{
				sm_AnimationShader->SetMat4("finalBonesMatrices[" + std::to_string(i) + "]", glm::mat4(1.0f));
			}
		}

		if (!m_Path.empty() && meshRendererComponent)
		{
			m_Animation.SetAnimation(m_Path, meshRendererComponent->GetModel());
			m_Animator.PlayAnimation(&m_Animation);
		}
	}

	void AnimatorComponent::OnComponentRemoved()
	{
		CRESTA_INFO("Animator Component OnComponentAdded");
	}
}