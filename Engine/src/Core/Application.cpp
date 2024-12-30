#include "Application.hpp"
#include "Core/Application.hpp"
#include "Crestaph.hpp"
#include "Renderer/Renderer.hpp"

namespace Cresta
{
	Cresta::Application* Cresta::Application::s_Instance = nullptr;
	
	Application::Application() 
	{
		Log::Init();
		Application::s_Instance = this;
		m_Running = true;
		m_Minimized = false;
		m_Window = Window::Create();
		m_ImGUILayer = new ImGUILayer();
		PushOverlay(m_ImGUILayer);
		m_Window->SetEventCallBack(CRESTA_BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{
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
		while (m_Running)
		{
			float time = RenderCommand::GetTime();
			float timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;
			
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