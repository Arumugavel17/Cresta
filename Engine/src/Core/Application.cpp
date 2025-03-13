#include "Application.hpp"
#include "Core/Time.hpp"
#include "Platform/OpenGL/Utils.hpp"
#include "ECS/Scene/SceneSerializer.hpp"

#include "Crestaph.hpp"

namespace Cresta
{
	Application* Application::sp_Instance = nullptr;
	Ref<Scene> Application::sp_ActiveScene = nullptr;
	
	Application::Application() : p_Running(true), p_Minimized(false)
	{
		Application::sp_Instance = this;
		p_Window = Window::Create();
		p_Window->SetEventCallBack(CRESTA_BIND_EVENT_FN(Application::OnEvent));

		Log::Init();
		Renderer::Init();
		Init();
	}

	void Application::Init()
	{
		sp_ActiveScene = CreateRef<Scene>();
		p_ImGUILayer = new ImGUILayer(sp_ActiveScene);

		PushOverlay(p_ImGUILayer);
	}

	Application::~Application()
	{
		Log::Shutdown();
		Renderer::Shutdown();
	}
	
	void Application::PushLayer(Layer* layer)
	{
		p_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		p_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Run()
	{
		CRESTA_PROFILE_FUNCTION();
		const float fixedTimestep = 1.0f / 60.0f; 
		float accumulatedTime = 0.0f;

		int frameCount = 0;
		float fpsTimer = 0.0f;          
		float currentFPS = 0.0f;        

		int fixedUpdateCount = 0;       

		while (p_Running)
		{
			float time = RenderCommand::GetTime();	 
			float timestep = time - p_LastFrameTime; 
			p_LastFrameTime = time;

			accumulatedTime += timestep;
			fpsTimer += timestep;       
			frameCount++;               

			// Update FPS every second
			if (fpsTimer >= 1.0f)
			{
				CRESTA_INFO("Update Count: {0} || Fixed Update Count: {1}", frameCount, fixedUpdateCount);

				currentFPS = frameCount;           
				frameCount = 0;                    
				fpsTimer = 0.0f;                   

				//CRESTA_INFO("FPS: {0}", currentFPS);
				fixedUpdateCount = 0;
			}

			while (accumulatedTime >= fixedTimestep)
			{
				for (Layer* layer : p_LayerStack)
				{
					layer->OnFixedUpdate();
				}
				accumulatedTime -= fixedTimestep;
				fixedUpdateCount++;
			}

			p_Window->Begin();

			if (!p_Minimized)
			{
				for (Layer* layer : p_LayerStack)
				{
					layer->OnUpdate();
				}

				p_ImGUILayer->Begin();
				{
					for (Layer* layer : p_LayerStack)
					{
						layer->OnImGUIRender();
					}
				}
				p_ImGUILayer->End();
			}

			p_Window->End();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(CRESTA_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(CRESTA_BIND_EVENT_FN(Application::OnWindowResize));
		for (auto it = p_LayerStack.rbegin(); it != p_LayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::NewScene()
	{
		sp_ActiveScene = CreateRef<Scene>();
		SetSceneForLayers();
	}

	void Application::OpenScene()
	{
		std::string filepath = Utils::FileDialogs::OpenScene("Cresta Scene (*.cresta)\0*.cresta\0");
		if (!filepath.empty())
		{
			return OpenScene(filepath);
		}
	}

	void Application::OpenScene(const std::filesystem::path& path)
	{
	
		if (path.extension().string() != ".cresta")
		{
			CRESTA_CORE_WARN("Could not load {0} - not a scene file", path.filename().string());
			return;
		}

		Ref<Scene> newScene = CreateRef<Scene>();
		if (SceneSerializer::Deserialize(*newScene ,path.string()))
		{
			//Ref<Scene> temp = sp_ActiveScene;
			sp_ActiveScene = newScene;
			p_ActiveScenePath = path;
			SetSceneForLayers();
			return;
		}

		CRESTA_CORE_WARN("Could not load {0} - not a scene file", path.filename().string());
	}

	//All the layer has a local Reference to the scene this method updates all the Layer
	void Application::SetSceneForLayers()
	{
		for (Layer* layer : p_LayerStack)
		{
			layer->SetScene(sp_ActiveScene);
		}
	}

	void Application::SaveScene()
	{
		if (!p_ActiveScenePath.empty())
		{
			sp_ActiveScene->SerializeScene(p_ActiveScenePath);
		}
		else
		{
			SaveSceneAs();
		}
	}

	void Application::SaveSceneAs()
	{
		std::string filepath = Utils::FileDialogs::SaveScene("Cresta Scene (*.cresta)\0*.cresta\0");
		if (!filepath.empty())
		{
			sp_ActiveScene->SerializeScene(filepath);
			p_ActiveScenePath = filepath;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) 
	{
		p_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e) 
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			p_Minimized = true;
			return false;
		}
		p_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return true;	
	}

	void Application::Close()
	{
		p_Running = false;
	}
}