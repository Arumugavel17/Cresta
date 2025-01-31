#include "Camera.hpp"
#include "Camera.hpp"
#include "Camera.hpp"

namespace Cresta {

	Camera::Camera(const glm::vec3& CameraPosition, CameraMode cameramode ) 
		: m_CameraFront(CameraPosition), 
		  m_CameraMode(CameraMode::Perspective)
	{

	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//sets the variables for Orthographic projection///////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////

	void Camera::SetOrthoGraphicView(
		float left, float right, 
		float bottom, float top, 
		float nearplane, float farplane)
	{
		m_OrthographicData = new OrthographicData(left, right, bottom, top, nearplane, farplane);
	}

	void Camera::SetOrthoGraphicView(OrthographicData& OrthoProjectionData)
	{
		m_OrthographicData = &OrthoProjectionData;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//sets the variables for Perspective projection////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////

	void Camera::SetPerspectiveView(
		float FOV, 
		float screenwidth, float screenheight, 
		float nearplane, float farplane)
	{
		m_PerspectiveData = new PerspectiveData(FOV, screenwidth, screenheight, nearplane, farplane);
	}

	void Camera::SetPerspectiveView(PerspectiveData& PerspectiveProjectionData)
	{
		m_PerspectiveData = &PerspectiveProjectionData;
	}

	void Camera::SetCameraWidthHeight(float width, float height)
	{
		m_PerspectiveData->m_Width = width;
		m_PerspectiveData->m_Height = height;

		CalculateViewMatrix();
		CalculateProjectionMatrix();
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//Helper Function//////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////

	const glm::vec3 Camera::GetPosition()
	{
		return m_CameraPos;
	}

	const void Camera::SetPosition(glm::vec3& position)
	{
		m_CameraPos = position;
	}

	const glm::mat4 Camera::GetProjectionMatrix() const 
	{
		return m_ProjectionMatrix; 
	}

	const glm::mat4 Camera::GetViewMatrix() const 
	{ 
		return m_ViewMatrix; 
	}

	const glm::mat4 Camera::GetViewProjectionMatrix() const 
	{ 
		return m_ViewProjectionMatrix; 
	}

	const CameraMode Camera::GetCameraMode() const 
	{ 
		return m_CameraMode; 
	}

	const void Camera::SetCameraMode(CameraMode mode)
	{
		m_CameraMode = mode;
	}

	glm::mat4& Camera::CalculateProjectionMatrix()
	{
		if (m_CameraMode == CameraMode::Perspective)
		{

			if (!m_PerspectiveData)
			{
				m_PerspectiveData = new PerspectiveData();
			}

			m_ProjectionMatrix = glm::perspective(m_PerspectiveData->m_FOV,
				m_PerspectiveData->m_Width / m_PerspectiveData->m_Height,
				m_PerspectiveData->m_NearPlane,
				m_PerspectiveData->m_FarPlane);
		}
		else if (m_CameraMode == CameraMode::OrthoGraphic)
		{
			if (!m_OrthographicData)
			{
				m_OrthographicData = new OrthographicData();
			}

			m_ProjectionMatrix = glm::ortho(m_OrthographicData->m_Left,
				m_OrthographicData->m_Right,
				m_OrthographicData->m_Bottom,
				m_OrthographicData->m_Top);
		}

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

		return m_ProjectionMatrix;
	}

	glm::mat4& Camera::CalculateViewMatrix() 
	{
		m_ViewMatrix = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

		return m_ViewMatrix;
	}

	void Camera::OnUpdate()
	{

	}

	void Camera::OnEvent(Event& e)
	{

	}
}