#pragma once
#include "Core/Window.hpp"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>
#include "Core/Events/WindowEvent.hpp"
#include "Core/Events/KeyBoardEvent.hpp"
#include "Core/Events/MouseEvent.hpp"

namespace Cresta {
	class OpenGLWindow : public Window {
	public:
		~OpenGLWindow();
		OpenGLWindow();
		void OnUpdate() override;
		void Init() override;
		void SetEventCallBack(EventCallBackFn fn);
		void OnShutDown() override;
		void SetVSync(bool enabled);
		void* GetWindowContext();
	private:
		EventCallBackFn CallBackFn;
		GLFWwindow* m_Window;
	};
}