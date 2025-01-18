#include "Core/Application.hpp"
#include "Core/Time.hpp"

#include "Crestaph.hpp"

namespace Cresta
{
	Application* Application::s_Instance = nullptr;
	
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
		m_ActiveScene = CreateRef<Scene>();
		m_PhysicsLayer = new PhysicsLayer(m_ActiveScene);
		m_ImGUILayer = new ImGUILayer(m_ActiveScene);

		PushLayer(m_PhysicsLayer);
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
		const float fixedTimestep = 1.0f / 60.0f; // 60 updates per second
		float accumulatedTime = 0.0f;

		int frameCount = 0;             // Counts the number of frames
		float fpsTimer = 0.0f;          // Timer to measure FPS
		float currentFPS = 0.0f;        // Current FPS value

		int fixedUpdateCount = 0;       // Tracks the number of FixedUpdate calls

		while (m_Running)
		{
			float time = RenderCommand::GetTime(); // Current time
			float timestep = time - m_LastFrameTime; // Elapsed time since last frame
			m_LastFrameTime = time;

			accumulatedTime += timestep;
			fpsTimer += timestep;       // Increment the FPS timer
			frameCount++;               // Increment the frame count

			// Update FPS every second
			if (fpsTimer >= 1.0f)
			{
				currentFPS = frameCount;           // Calculate FPS
				frameCount = 0;                    // Reset frame count
				fpsTimer = 0.0f;                   // Reset timer

				// Log the FPS and fixed update count
				std::cout << "FPS: " << currentFPS << ", Fixed Updates: " << fixedUpdateCount << std::endl;

				// Reset fixed update count for display purposes
				fixedUpdateCount = 0;
			}

			// Process fixed updates
			while (accumulatedTime >= fixedTimestep)
			{
				// Call FixedUpdate on layers
				for (Layer* layer : m_LayerStack)
				{
					layer->OnFixedUpdate();
				}
				accumulatedTime -= fixedTimestep;
				fixedUpdateCount++; // Increment FixedUpdate counter
			}

			m_Window->Begin();

			if (!m_Minimized)
			{
				// Regular update
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

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e) {

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