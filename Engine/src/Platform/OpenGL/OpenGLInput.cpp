#include "OpenGLInput.hpp"
#include "Core/Application.hpp"

namespace Cresta {

    bool OpenGLInput::GetKeyDownImpl(KeyCode key)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow()->GetWindowContext());
        auto state = glfwGetKey(window, key);
        return state == GLFW_PRESS;
    }

    bool OpenGLInput::GetKeyUpImpl(KeyCode key)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow()->GetWindowContext());
        auto state = glfwGetKey(window, key);
        return state == GLFW_RELEASE;
    }

    bool OpenGLInput::GetMouseDownImpl(MouseCode Code)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow()->GetWindowContext());
        auto state = glfwGetMouseButton(window, Code);
        return state == GLFW_PRESS;
    }

    bool OpenGLInput::GetMouseUpImpl(MouseCode Code)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow()->GetWindowContext());
        auto state = glfwGetMouseButton(window, Code);
        return state == GLFW_RELEASE;
    }

    std::pair<float, float> OpenGLInput::GetMousePositionImp()
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetApplication().GetWindow()->GetWindowContext());
        double xpos = 0, ypos = 0;
        glfwGetCursorPos(window, &xpos, &ypos);
        return { (float)xpos, (float)ypos};
    }
}