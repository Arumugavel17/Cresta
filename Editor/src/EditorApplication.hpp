#pragma once
#include "Core/Application.hpp"
#include "EditorLayer.hpp"

namespace Cresta {
	class EditorApplication : public Application {
	public:
		EditorApplication()
		{
			EditorLayer* t_EditorLayer = new EditorLayer();
			PushLayer(t_EditorLayer);
			Init();
		}

		~EditorApplication() = default;

		void Init();
	};

	Application* CreateApplication() {
		return new EditorApplication();
	}
}