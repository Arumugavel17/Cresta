#pragma once
#include "ApplicationHeader.hpp"

namespace Cresta 
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Run();
		virtual void OnEvent(Event& e);
		
		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);

		void SaveScene();
		void SaveSceneAs();
		void SerializeScene(const Ref<Scene>& scene, const std::filesystem::path& path);
		void SetSceneForLayers();

		void Init();
		void Close();
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		
		Ref<Window> GetWindow() { return p_Window; }
		static Application& GetApplication() { return *Application::sp_Instance; }

	protected:
		float p_LastFrameTime = 0.0f;
		bool p_Running;
		bool p_Minimized;
		
		std::filesystem::path p_ActiveScenePath;

		static Ref<Scene> sp_ActiveScene;
		static Application* sp_Instance; 

		Ref<Window> p_Window;
		ImGUILayer* p_ImGUILayer;
		LayerStack p_LayerStack;
	};

	Application* CreateApplication();
}