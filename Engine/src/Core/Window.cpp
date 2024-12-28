#include "Window.hpp"
#include "Window.hpp"
#include "Platform/OpenGL/OpenGLWindow.hpp"


namespace Cresta {
	void Window::ResizeViewPort()
	{

	}

	Window* Window::Create()
	{
		return new OpenGLWindow();
	}
}