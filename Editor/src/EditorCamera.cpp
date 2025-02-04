#include "EditorCamera.hpp"
#include "Core/Input.hpp"
#include "Renderer/Renderer.hpp"

namespace Cresta {
	EditorCamera::EditorCamera(const glm::vec3& CameraPosition, CameraMode cameramode)
		: Camera(CameraPosition,cameramode)
	{
        m_CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        yaw = -90.0f;  // Starting yaw so the camera points forward
        pitch = 0.0f;  // Starting pitch level

        CalculateViewMatrix();
        CalculateProjectionMatrix();
	}

	void EditorCamera::OnUpdate()
	{
        if(m_CameraMovementEnabled && Input::GetKeyDown(Key::LeftAlt))
        {
            CameraMovement();
            CameraRotation();
        }
        else 
        {
            std::pair<float, float> mouseXY = Input::GetMousePosition();
            lastX = mouseXY.first;
            lastY = mouseXY.second;
        }

	}

    void EditorCamera::CameraRotation() 
    {
        std::pair<float,float> mouseXY = Input::GetMousePosition();

        glm::vec3 t_CamerFront = m_CameraFront; // chache the cmaera Front
        double xoffset = mouseXY.first - lastX;
        double yoffset = lastY - mouseXY.second;
        lastX = mouseXY.first;
        lastY = mouseXY.second;

        double sensitivity = 0.4f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        m_CameraFront = glm::normalize(direction);

        if (t_CamerFront != m_CameraFront)
        {
            CalculateViewMatrix();
            CalculateProjectionMatrix();       
        }
    }

    glm::vec3 EditorCamera::GetCameraFront()
    {
        return m_CameraFront;
    }

    void EditorCamera::SetCameraMovementEnabled(bool enable)
    {
        m_CameraMovementEnabled = enable;
    }

    bool EditorCamera::GetCameraMovementEnabled()
    {
        return m_CameraMovementEnabled;
    }


	void EditorCamera::OnEvent(Event& e)
	{
    }

    void EditorCamera::CameraMovement() 
    {
        glm::vec3 t_CamerPos = m_CameraPos; // chache the cmaera position
        float currenttime = RenderCommand::GetTime();
        float deltatime = currenttime - lasttime;
        float speed_factor = 0.5f;
        lasttime = currenttime;

        if (Input::GetKeyDown(Key::LeftShift))
        {
            speed_factor = 10;
        }

        else if (Input::GetKeyDown(Key::LeftControl))
        {
            speed_factor = 2.5f;
        }

        const float cameraSpeed = speed_factor * deltatime; // adjust accordingly

        if (Input::GetKeyDown(Key::A))
        {
            m_CameraPos -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
        }
        if (Input::GetKeyDown(Key::D))
        {
            m_CameraPos += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
        }
        if (Input::GetKeyDown(Key::S))
        {
            m_CameraPos -= m_CameraFront * cameraSpeed;
        }
        if (Input::GetKeyDown(Key::W))
        {
            m_CameraPos += m_CameraFront * cameraSpeed;
        }
        if (Input::GetKeyDown(Key::E))
        {
            m_CameraPos += glm::normalize(m_CameraUp) * cameraSpeed;
        }
        if (Input::GetKeyDown(Key::Q))
        {
            m_CameraPos += glm::normalize(-m_CameraUp) * cameraSpeed;
        }

        //Only update if Camera Position changes
        if (t_CamerPos != m_CameraPos)
        {
            CalculateViewMatrix();
            CalculateProjectionMatrix();
        }
    }

}