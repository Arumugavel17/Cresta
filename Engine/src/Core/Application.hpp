#pragma once
#include "Core/Window.hpp"
#include "Core/Input.hpp"
#include "Core/Events/Event.hpp"
#include "Core/Events/WindowEvent.hpp"
#include "Core/Events/KeyBoardEvent.hpp"
#include "Core/Events/MouseEvent.hpp"
#include "Core/Layer.hpp"
#include "Core/LayerStack.hpp"
#include <iostream>
#include <memory>

namespace Cresta {
	class Application{
	public:
		Application() {
			Log::Init();
			Application::s_Instance = this;
			m_Running = true;
			m_Window = Window::Create();
		};

		virtual ~Application() = default;
		virtual void Init() = 0;
		virtual void Run() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnEvent(Event& e) = 0;
		virtual void FixedUpdate() = 0;
		virtual void OnImGUI() = 0;

		static bool OnWindowClose(WindowCloseEvent& e);
		static bool OnWindowResize(WindowResizeEvent& e);
		
		Window& GetWindow() { return *m_Window; }
		static Application& GetApplication() { return *Application::s_Instance; }
	protected:
		Window* m_Window;
		bool m_Running;
		static Application* s_Instance;
		LayerStack m_LayerStack;
	};

	Application* CreateApplication();
}