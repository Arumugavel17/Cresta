#include "EditorApplication.hpp"

namespace Cresta {
	EditorApplication::EditorApplication()
	{
		Init();
	}

	void EditorApplication::Init()
	{
		m_Window->SetEventCallBack(CRESTA_BIND_EVENT_FN(EditorApplication::OnEvent));
	}

	void EditorApplication::Run()
	{
		while (true)
		{
			for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
			{
				(*it)->OnUpdate();
			}

			m_Window->OnUpdate();
		}
	}
	void EditorApplication::OnUpdate()
	{

	}

	void EditorApplication::OnEvent(Event& e)
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
	void EditorApplication::FixedUpdate()
	{
	}
	void EditorApplication::OnImGUI()
	{
	}
}
