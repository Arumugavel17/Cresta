#include "Core/Application.hpp"

namespace Cresta {
	Cresta::Application* Cresta::Application::s_Instance = nullptr;
	
	bool Application::OnWindowClose(WindowCloseEvent& e) {
		return false;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e) {
		
		return false;	
	}
}