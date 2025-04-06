#include "Crestaph.hpp"
#include "Components.hpp"
#include "Renderer/Animation/Animator.hpp"

namespace Cresta
{
	class MeshRenderer;
	class AnimatorComponent : ComponentTemplate
	{
	public:
		AnimatorComponent(Entity* entity) : ComponentTemplate(entity)
		{
			if (!sm_AnimationShader)
			{
				sm_AnimationShader = Shader::Create("assets/shaders/Animation.glsl");
			}
		}
		inline void const SetPath(std::string path) { m_Path = path; PathChanged(); }

		void UI() override;
		void OnStart() override;
		void OnEnd() override;
		void OnUpdate() override;
		void OnComponentAdded() override;
		void OnComponentRemoved() override;
		std::string GetPath() { return m_Path; }

		std::string ToString() override { return "Mesh Renderer"; }
		void PathChanged();

	private:
		void UpdateAnimation();

		MeshRenderer* meshRendererComponent;
		std::string m_Path;
		Animator m_Animator;
		Animation m_Animation;
		static Ref<Shader> sm_AnimationShader;
	};
}