#include "OpenGLInput.hpp"
#include "Core/Application.hpp"

namespace Cresta 
{
#define GET_WINDOW static_cast<GLFWwindow*>(Application::GetApplication().GetWindow()->GetWindowContext());

    bool OpenGLInput::GetKeyDownImpl(KeyCode key)
    {
        GLFWwindow* window = GET_WINDOW;
        auto state = glfwGetKey(window, key);
        return state == GLFW_PRESS;
    }

    bool OpenGLInput::GetKeyUpImpl(KeyCode key)
    {
        GLFWwindow* window = GET_WINDOW;
        auto state = glfwGetKey(window, key);
        return state == GLFW_RELEASE;
    }

    bool OpenGLInput::GetMouseDownImpl(MouseCode Code)
    {
        GLFWwindow* window = GET_WINDOW;
        auto state = glfwGetMouseButton(window, Code);
        return state == GLFW_PRESS;
    }

    bool OpenGLInput::GetMouseUpImpl(MouseCode Code)
    {
        GLFWwindow* window = GET_WINDOW;
        auto state = glfwGetMouseButton(window, Code);
        return state == GLFW_RELEASE;
    }

    std::pair<float, float> OpenGLInput::GetMousePositionImp()
    {
        GLFWwindow* window = GET_WINDOW;
        double xpos = 0, ypos = 0;
        glfwGetCursorPos(window, &xpos, &ypos);
        return { (float)xpos, (float)ypos};
    }
}