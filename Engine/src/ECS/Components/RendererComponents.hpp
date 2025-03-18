#pragma once
#include "Crestaph.hpp"
#include "Components.hpp"
#include "Renderer/Animation/Animator.hpp"
#include "Core/Time.hpp"
#include "Renderer/RendererCommand.hpp"

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
	private:
		int m_ID;
		std::string m_Path;
		Ref<Model> m_Model;

		void PathChanged()
		{
			if (!m_Path.empty())
			{
				m_Model = Model::Create(m_Path);
			}
		}

	public:
		MeshRenderer(Entity* entity, const std::string& path = std::string(), int id = -1);

		inline void const SetID(int ID) { m_ID = ID; }
		inline void const SetPath(std::string path) { m_Path = path; }
	
		void const Draw(const glm::mat4& transform) 
		{ 
			if (m_Model)
			{
				m_Model->Draw(transform, m_ID);
			}
		}

		inline const int GetID() const { return m_ID; }
		inline const std::string& GetPath() const { return m_Path; }
		inline const Ref<Model>& GetModel() const { return m_Model; }

		void OnRender() override;
		void OnComponentAdded() override;
		void OnComponentRemoved() override;

		void UI() override;
		float time = 0;
		std::string ToString() override { return "Mesh Renderer"; }
	};

	class AnimatorComponent : ComponentTemplate
	{
	public:
		AnimatorComponent(Entity* entity) : ComponentTemplate(entity){}
		inline void const SetPath(std::string path) { m_Path = path; }

		void UI() override;
		void OnUpdate() override;
		void OnComponentAdded() override;
		void OnComponentRemoved() override;

		std::string ToString() override { return "Mesh Renderer"; }
		void PathChanged()
		{
			if (!m_Path.empty())
			{
				m_Animation.SetAnimation(m_Path,m_Model);
			}
		}

	private:
		void UpdateAnimation();

		std::string m_Path;
		Ref<Model> m_Model;
		Animator m_Animator;
		Animation m_Animation;
	};
}