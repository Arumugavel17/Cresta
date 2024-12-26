#include "Window.hpp"
#include "Platform/OpenGL/OpenGLWindow.hpp"


namespace Cresta {

	Window* Window::Create()
	{
#pragma message("Debug mode is enabled")
		std::cout << "OpenGLWindow Created";
		return new OpenGLWindow();
	}
}