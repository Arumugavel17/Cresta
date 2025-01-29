#include "Application.hpp"
#include "Core/Time.hpp"
#include "Scene/SceneSerializer.hpp"
#include "Platform/OpenGL/Utils.hpp"

#include "Crestaph.hpp"

namespace Cresta
{
	Application* Application::s_Instance = nullptr;
	Ref<Scene> Application::s_ActiveScene = nullptr;
	
	Application::Application() : m_Running(true), m_Minimized(false)
	{
		Application::s_Instance = this;
		m_Window = Window::Create();
		m_Window->SetEventCallBack(CRESTA_BIND_EVENT_FN(Application::OnEvent));

		Log::Init();
		Renderer::Init();
		Init();
	}

	void Application::Init()
	{
		s_ActiveScene = CreateRef<Scene>();
		m_ImGUILayer = new ImGUILayer(s_ActiveScene);

		PushOverlay(m_ImGUILayer);
	}

	Application::~Application()
	{
		Log::Shutdown();
		Renderer::Shutdown();
	}
	
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Run()
	{
		const float fixedTimestep = 1.0f / 60.0f; 
		float accumulatedTime = 0.0f;

		int frameCount = 0;
		float fpsTimer = 0.0f;          
		float currentFPS = 0.0f;        

		int fixedUpdateCount = 0;       

		while (m_Running)
		{
			float time = RenderCommand::GetTime();	 
			float timestep = time - m_LastFrameTime; 
			m_LastFrameTime = time;

			accumulatedTime += timestep;
			fpsTimer += timestep;       
			frameCount++;               

			// Update FPS every second
			if (fpsTimer >= 1.0f)
			{
				currentFPS = frameCount;           
				frameCount = 0;                    
				fpsTimer = 0.0f;                   

				//CRESTA_INFO("FPS: {0}", currentFPS);
				fixedUpdateCount = 0;
			}

			while (accumulatedTime >= fixedTimestep)
			{
				for (Layer* layer : m_LayerStack)
				{
					layer->OnFixedUpdate();
				}
				accumulatedTime -= fixedTimestep;
				fixedUpdateCount++;
			}

			m_Window->Begin();

			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
				{
					layer->OnUpdate();
				}

				m_ImGUILayer->Begin();
				{
					for (Layer* layer : m_LayerStack)
					{
						layer->OnImGUIRender();
					}
				}
				m_ImGUILayer->End();
			}

			m_Window->End();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(CRESTA_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(CRESTA_BIND_EVENT_FN(Application::OnWindowResize));
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::NewScene()
	{
		s_ActiveScene = CreateRef<Scene>();
		SetSceneForLayers();
	}

	void Application::OpenScene()
	{
		std::string filepath = Utils::FileDialogs::OpenFile("Cresta Scene (*.cresta)\0*.cresta\0");
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
		SceneSerializer serializer(newScene);
		if (serializer.Deserialize(path.string()))
		{
			Ref<Scene> temp = s_ActiveScene;
			CRESTA_INFO("Scene Reference Before {0}", temp.use_count());
			s_ActiveScene = newScene;
			m_ActiveScenePath = path;
			SetSceneForLayers();
			CRESTA_INFO("Scene Reference After {0}", temp.use_count());
			return;
		}

		CRESTA_CORE_WARN("Could not load {0} - not a scene file", path.filename().string());
		return;
	}

	void Application::SerializeScene(Ref<Scene> scene, const std::filesystem::path& path)
	{
		SceneSerializer serializer(scene);
		serializer.Serialize(path.string());
	}

	void Application::SetSceneForLayers()
	{
		for (Layer* layer : m_LayerStack)
		{
			layer->SetScene(s_ActiveScene);
		}
	}

	void Application::SaveScene()
	{
		if (!m_ActiveScenePath.empty())
		{
			SerializeScene(s_ActiveScene, m_ActiveScenePath);
		}
		else
		{
			SaveSceneAs();
		}
	}

	void Application::SaveSceneAs()
	{
		std::string filepath = Utils::FileDialogs::SaveFile("Cresta Scene (*.cresta)\0*.cresta\0");
		if (!filepath.empty())
		{
			SerializeScene(s_ActiveScene, filepath);
			m_ActiveScenePath = filepath;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) 
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e) 
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return true;	
	}

	void Application::Close()
	{
		m_Running = false;
	}
}