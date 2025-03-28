#pragma once
#include "Crestaph.hpp"
#include "Components.hpp"
#include "Renderer/Animation/Animator.hpp"
#include "ECS/UUID.hpp"

namespace Cresta
{	
	class SpriteRenderer : public ComponentTemplate
	{
	public:
		SpriteRenderer(Entity* entity,const glm::vec4& color = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f)) : ComponentTemplate(entity), Color(color) {}
		void UI() override;

		void SetPath(const std::string& path)
		{
			std::copy(path.begin(), path.end(), m_Path);
			m_Path[path.size()] = '\0';
			PathChanged();
		}
		inline std::string GetPath() const { return m_Path; }
		
		std::string ToString() override
		{
			return "Sprite Renderer Component";
		}

		void OnComponentAdded() override;
		void OnComponentRemoved() override;

	private:
		void PathChanged()
		{
			try
			{
				Ref<Texture2D> ref = Texture2D::Create(m_Path);
				Texture = ref;
			}
			catch (std::exception e)
			{
				CRESTA_CORE_ERROR(e.what());
			}
		}
	public:
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		char* m_Path = new char[128]();
		float MixFactor = 1.0f;
	};

	
	class MeshRenderer : public ComponentTemplate
	{
	public:
		enum class ShaderType {
			NORMAL,
			ANIMATION
		};

		MeshRenderer(Entity* entity, const std::string& path = std::string(), int64_t = -1);

		inline void const SetID(int ID) { m_ID = ID; }
		inline void const SetPath(std::string path) 
		{ 
			m_Path = path; 
			PathChanged();
		}
	
		void const Draw(const glm::mat4& transform) 
		{ 
			if (m_Model)
			{
				if (sm_Shader.find(m_ShaderType) == sm_Shader.end()) {
					std::cerr << "Shader NORMAL not found!" << std::endl;
				}
				else {
					m_Model->Draw(sm_Shader[m_ShaderType], transform, m_ID);
				}
			}
		}

		void CreateModelDataFile();
		inline const int GetID() const { return m_ID; }
		inline const std::string& GetPath() const { return m_Path; }
		inline const Ref<Model>& GetModel() const { return m_Model; }

		void OnRender() override;
		void OnComponentAdded() override;
		void OnComponentRemoved() override;
		
		void SetShaderType(ShaderType type) { m_ShaderType = type; }
		void SetShader(ShaderType type, Ref<Shader> shader) { sm_Shader[type] = shader; }
		void UI() override;
		std::string ToString() override { return "Mesh Renderer"; }

	private:
		UUID* m_ModelID;
		int m_ID;
		std::string m_Path;
		Ref<Model> m_Model;
		static std::map<ShaderType,Ref<Shader>> sm_Shader;
		ShaderType m_ShaderType = ShaderType::NORMAL;

		void PathChanged()
		{
			if (!m_Path.empty())
			{
				m_Model = Model::Create(m_Path, *m_ModelID);
			}
		}
	};


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