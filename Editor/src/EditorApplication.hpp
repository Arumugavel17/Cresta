#pragma once
#include "Core/Application.hpp"
#include "EditorLayer.hpp"

namespace Editor 
{
	class EditorApplication : public Cresta::Application 
	{
	public:
		EditorApplication()
		{
			m_EditorLayer = new EditorLayer(sp_ActiveScene);
			PushLayer(m_EditorLayer);
		}

		~EditorApplication() = default;

		void SaveProject() override;
		void NewProject() override;
		void OpenProject() override;
		std::string GetProjectFolder();

		void OnEvent(Event& e) override;
		void Run() override;
	private:
		EditorLayer* m_EditorLayer;
	};
}

namespace Cresta
{
	Application* CreateApplication()
	{
		return new Editor::EditorApplication();
	}
}