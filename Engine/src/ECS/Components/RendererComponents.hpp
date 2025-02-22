#pragma once
#include "Crestaph.hpp"
#include "Components.hpp"

namespace Cresta
{
	class SpriteRenderer : public Component
	{
	public:
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;

		char* path = new char[128]();
		float MixFactor = 1.0f;
		SpriteRenderer() = default;
		SpriteRenderer(const glm::vec4& color)
			: Color(color) {
		}

		void PathChanged()
		{
			try
			{
				Ref<Texture2D> ref = Texture2D::Create(path);
				Texture = ref;
			}
			catch (std::exception e)
			{
				CRESTA_CORE_ERROR(e.what());
			}
		}

		std::string ToString() override
		{
			return "Sprite Renderer Component";
		}

		void OnComponentAdded(Entity& entity) override;
		void OnComponentRemoved(Entity& entity) override;
	};

	class MeshRenderer : public Component
	{
	private:
		int m_ID;
		std::string m_Path;
		Ref<Model> m_Model;

	public:
		MeshRenderer() = default;
		MeshRenderer(std::string path, int id) : m_Path(path), m_ID(id)
		{
			PathChanged();
		}

		void PathChanged()
		{
			if (m_Path != "")
			{
				m_Model = Model::Create(m_Path);
			}
		}

		void const SetID(int ID) { m_ID = ID; }
		void const SetPath(std::string path) { m_Path = path; }
		void const SetPath(char* path) { m_Path = std::string(path); }

		inline const int GetID() const { return m_ID; }
		inline const std::string& GetPath() const { return m_Path; }
		inline const Ref<Model>& GetModel() const { return m_Model; }

		void OnComponentAdded(Entity& entity) override;
		void OnComponentRemoved(Entity& entity) override;

		std::string ToString() override { return "Mesh Renderer"; }
	};
}