#include "Window.hpp"
#include "Window.hpp"
#include "Platform/OpenGL/OpenGLWindow.hpp"


namespace Cresta {
	void Window::ResizeViewPort()
	{

	}

	Ref<Window> Window::Create()
	{
		return CreateRef<OpenGLWindow>();
	}
}