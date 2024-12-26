#pragma once
#include "Core/Events/Event.hpp"
#include "Core/Application.hpp"

namespace Cresta {
	class EditorApplication : public Application {
	public:
		EditorApplication();
		void Init();
		void Run();
		void OnUpdate();
		void OnEvent(Event& e);
		void FixedUpdate();
		void OnImGUI();
	};

	Application* CreateApplication() {
		return new EditorApplication();
	}
}