#pragma once
#include "Core/Application.hpp"
#include "EditorLayer.hpp"

namespace Cresta {
	class EditorApplication : public Application {
	public:
		EditorApplication()
		{
			m_EditorLayer = new EditorLayer(s_ActiveScene);
			PushLayer(m_EditorLayer);
			Init();
		}

		~EditorApplication() = default;

		void Init();
	private:
		EditorLayer* m_EditorLayer;
	};

	Application* CreateApplication() {
		return new EditorApplication();
	}
}