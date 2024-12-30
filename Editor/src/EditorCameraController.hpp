#pragma once
#include "Renderer/Camera.hpp"
#include "Core/Events/Event.hpp"
#include "glm/glm.hpp"

namespace Cresta {
	class EditorCameraController {
	
	public:
		EditorCameraController(Camera& camera) : m_Camera(camera), m_Position(camera.GetPosition()) {}

		void OnUpdate();
		void OnEvent(Event& e);
	
	private:
		Camera& m_Camera;
		glm::vec3 m_Position;
	};
}