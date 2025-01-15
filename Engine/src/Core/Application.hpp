#pragma once
#include "Core/Window.hpp"
#include "Core/Input.hpp"
#include "Core/Events/Event.hpp"
#include "Core/Events/WindowEvent.hpp"
#include "Core/Events/KeyBoardEvent.hpp"
#include "Core/Events/MouseEvent.hpp"
#include "Core/Layer.hpp"
#include "Core/LayerStack.hpp"
#include "Renderer/Renderer.hpp"
#include "Core/ImGUILayer.hpp"
#include "Physics/Physics.hpp"
#include <iostream>
#include <memory>

namespace Cresta {
	class Application{
	public:
		Application();
		virtual ~Application();
		virtual void Run();
		virtual void OnEvent(Event& e);
		
		void Init();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		void Close();
		
		Window& GetWindow() { return *m_Window; }
		static Application& GetApplication() { return *Application::s_Instance; }

	protected:
		float m_LastFrameTime = 0.0f;
		Window* m_Window;
		bool m_Running;
		bool m_Minimized;
		ImGUILayer* m_ImGUILayer;
		static Application* s_Instance;
		LayerStack m_LayerStack;
	};

	Application* CreateApplication();
}