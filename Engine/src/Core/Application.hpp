#pragma once
#include "Core/Events/Event.hpp"
#include "Core/Window.hpp"
#include "Core/Input.hpp"
#include <iostream>
#include <memory>

namespace Cresta {
	class Application{
	public:
		Application() {
			Application::s_Instance = this;
		};
		virtual ~Application() = default;
		virtual void Init() = 0;
		virtual void Run() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnEvent(Event& e) = 0;
		virtual void FixedUpdate() = 0;
		virtual void OnImGUI() = 0;
		Window* GetWindow() { return m_Window; }
		static Application* GetApplication() { return Application::s_Instance; }
	protected:
		Window* m_Window;
		bool m_Running;
		static Application* s_Instance;
	};

	Application* CreateApplication();
}