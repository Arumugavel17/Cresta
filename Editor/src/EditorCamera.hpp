#pragma once
#include "Renderer/Camera.hpp"
#include "Core/Events/KeyBoardEvent.hpp"
#include "Core/Events/MouseEvent.hpp"

namespace Cresta 
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera(const glm::vec3& CameraPosition = glm::vec3(0, 0, -1.0f),
					 CameraMode cameramode = CameraMode::Perspective);

		void OnUpdate() override;
		void OnEvent(Event& e) override;
		void CameraMovement();
		void CameraRotation();
		glm::vec3 GetCameraFront();
		void SetCameraMovementEnabled(bool enable);
		bool GetCameraMovementEnabled();

	private:
		bool m_CameraMovementEnabled = false;
		float m_LastTime = 0;
		float m_CurrentTime = 0;
		float m_DeltaTime = 0;
		float m_Zoom = 45.0f;
		double lastX = 0, lastY = 0, yaw = -90, pitch;
	};
}