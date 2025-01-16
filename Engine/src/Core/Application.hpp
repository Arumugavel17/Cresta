#pragma once
#include "ApplicationHeader.hpp"

namespace Cresta {
	class Application{
	public:
		Application();
		virtual ~Application();

		virtual void Run();
		virtual void OnEvent(Event& e);
		
		void Init();
		void Close();
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		
		Ref<Window> GetWindow() { return m_Window; }
		static Application& GetApplication() { return *Application::s_Instance; }

	protected:
		float m_LastFrameTime = 0.0f;
		bool m_Running;
		bool m_Minimized;

		Ref<Scene> m_ActiveScene;
		Ref<Window> m_Window;

		PhysicsLayer* m_PhysicsLayer;
		ImGUILayer* m_ImGUILayer;
		static Application* s_Instance; 

		LayerStack m_LayerStack;
	};

	Application* CreateApplication();
}