#include "EditorApplication.hpp"

namespace Cresta {
	EditorApplication::EditorApplication()
	{
		Init();
	}

	void EditorApplication::Init()
	{
		m_Running = true;
		m_Window = Window::Create();
		Input::Create();
		m_Window->SetEventCallBack(CRESTA_BIND_EVENT_FN(EditorApplication::OnEvent));
	}

	void EditorApplication::Run()
	{
		while (true)
		{
			m_Window->OnUpdate();
		}
	}
	void EditorApplication::OnUpdate()
	{

	}
	void EditorApplication::OnEvent(Event& e)
	{

	}
	void EditorApplication::FixedUpdate()
	{
	}
	void EditorApplication::OnImGUI()
	{
	}
}
