#include "OpenGLWindow.hpp"
#include "Renderer/RendererCommand.hpp"

#define HZ_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }


namespace Cresta {

	static void GLFWErrorCallback(int error, const char* description)
	{
		CRESTA_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	OpenGLWindow::~OpenGLWindow()
	{
		glfwDestroyWindow(m_Window);
	}

	OpenGLWindow::OpenGLWindow()
	{
		Init();
	}

	void OpenGLWindow::Init()
	{
		m_Data.Title = "Cresta";
		m_Data.Width = 1920;
		m_Data.Height = 1080;

		bool status = glfwInit();	
		CRESTA_ASSERT(!status,"Failed to Initalize GLFW");


		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

		m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), NULL, NULL);
		glfwSetWindowAttrib(m_Window, GLFW_MAXIMIZED, GLFW_TRUE);

		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		glfwMakeContextCurrent(m_Window);
		
		glfwSetErrorCallback(GLFWErrorCallback);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}
		
		glfwSetWindowUserPointer(m_Window, &m_Data);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.CallBackFunction(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.CallBackFunction(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key, 0);
						data.CallBackFunction(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						data.CallBackFunction(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, true);
						data.CallBackFunction(event);
						break;
					}
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(keycode);
				data.CallBackFunction(event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.CallBackFunction(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.CallBackFunction(event);
					break;
				}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.CallBackFunction(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.CallBackFunction(event);
			});
	}

	void OpenGLWindow::SetEventCallBack(EventCallBackFn fn)
	{
		m_Data.CallBackFunction = fn;
	}

	void OpenGLWindow::Begin() 
	{
		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
		RenderCommand::Clear();
	}

	void OpenGLWindow::End() 
	{
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	void OpenGLWindow::OnShutDown()
	{
		glfwTerminate();
	}

	void OpenGLWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
	}

	void* OpenGLWindow::GetWindowContext()
	{
		return (void*)m_Window;
	}
	uint32_t OpenGLWindow::GetWidth() const
	{
		return m_Data.Width;
	}
	uint32_t OpenGLWindow::GetHeight() const
	{
		return m_Data.Height;
	}
}