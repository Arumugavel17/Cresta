#pragma once
#include "Crestaph.hpp"
#include "Components.hpp"

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
		
		void OnUpdate() override
		{

		}

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

	public:
		MeshRenderer(Entity* entity, const std::string& path = std::string(), int id = -1);

		void PathChanged()
		{
			if (!m_Path.empty())
			{
				m_Model = Model::Create(m_Path);
			}
		}

		void const SetID(int ID) { m_ID = ID; }
		void const SetPath(std::string path) { m_Path = path; }
		void const SetPath(char* path) { m_Path = std::string(path); }

		void const Draw(const glm::mat4& transform) 
		{ 
			if (m_Model)
			{
				m_Model->Draw(transform,m_ID); 
			}
		}

		inline const int GetID() const { return m_ID; }
		inline const std::string& GetPath() const { return m_Path; }
		inline const Ref<Model>& GetModel() const { return m_Model; }

		void OnUpdate() override;
		void OnComponentAdded() override;
		void OnComponentRemoved() override;

		void UI() override;

		std::string ToString() override { return "Mesh Renderer"; }
	};
}