#pragma once
#include "Crestaph.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp> 
#include "Core/Events/Event.hpp"

namespace Cresta {

	struct OrthographicData 
	{
		float m_Left, m_Right, m_Bottom, m_Top, m_NearPlane, m_FarPlane;
	///-10.0f, 10.0f, -10.0f, 10.0f
		OrthographicData(float left = -10.0f, float right = 10.0f, 
			float bottom = -10.0f, float top = 10.0f, float nearplane = 0.0001f, float farplane = 100.0f)
		{
			m_Left		= left;
			m_Right		= right;
			m_Bottom	= bottom;
			m_Top		= top;
			m_NearPlane = nearplane;
			m_FarPlane	= farplane;
		}

		OrthographicData(OrthographicData& other){
			m_Left		= other.m_Left;
			m_Right		= other.m_Right;
			m_Bottom	= other.m_Bottom;
			m_Top		= other.m_Top; 
			m_NearPlane = other.m_NearPlane;
			m_FarPlane	= other.m_FarPlane;

		}

		bool isValid() const 
		{
			return m_Left != 0 || m_Right != 0 || m_Bottom != 0 || m_Top != 0;
		}
	};

	struct PerspectiveData 
	{
		float m_FOV, m_Width, m_Height, m_NearPlane, m_FarPlane;

		// FOV in degress
		PerspectiveData(float FOV = 45, float Width = 1920, float Height = 1080, float nearplane = 0.0001f, float farplane = 100.0f) {
			m_FOV		= glm::radians(FOV);
			m_Width		= Width;
			m_Height	= Height;
			m_NearPlane = nearplane;
			m_FarPlane	= farplane;
		}
		PerspectiveData(PerspectiveData& other) {
			m_FOV		= other.m_FOV;
			m_Width		= other.m_Width;
			m_Height	= other.m_Height;
			m_NearPlane = other.m_NearPlane;
			m_FarPlane	= other.m_FarPlane;
		}

		bool isValid() const 
		{
			return m_FOV != 0 || m_Width != 0 || m_Height != 0;
		}
	};

	enum class CameraMode 
	{
		OrthoGraphic,
		Perspective 
	};

	class Camera
	{
	public:
		Camera(glm::vec3& CameraPosition = glm::vec3(0.0f), 
			CameraMode cameramode = CameraMode::Perspective);

		~Camera() = default;

		// sets the variables for Orthographic projection
		void SetOrthoGraphicView(float left, float right, float bottom, float top, float nearplane, float farplane);
		void SetOrthoGraphicView(OrthographicData& OrthoProjectionData); 

		// sets the variables for Perspective projection
		void SetPerspectiveView(float FOV, float screenwidth, float screenheight, float nearplane, float farplane);
		void SetPerspectiveView(PerspectiveData& PerspectiveProjectionData); 
		

		// Helper Function
		const glm::vec3 GetPosition();
		const void SetPosition(glm::vec3& position);
		const glm::mat4& GetProjectionMatrix() const ;
		const glm::mat4& GetViewMatrix() const ; 
		const glm::mat4& GetViewProjectionMatrix() const ;
		const CameraMode GetCameraMode() const ;

		glm::mat4& CalculateProjectionMatrix();
		glm::mat4& CalculateViewMatrix();

		virtual void OnUpdate();
		virtual void OnEvent(Event& e);

	private:
		
		glm::vec3 m_CameraPos;
		glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		glm::vec3 m_EulerRotation;
		glm::quat m_QuatRotation;

		CameraMode m_CameraMode;
			
		OrthographicData* m_OrthographicData;
		PerspectiveData* m_PerspectiveData;
	};
}