#pragma once
#include "Core/Layer.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"
#include "Renderer/FrameBuffer.hpp"
#include "EditorCamera.hpp"

namespace Cresta {
	class EditorLayer : public Layer{
	public:
		EditorLayer();

		virtual void OnUpdate() override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;
		virtual void OnImGUIRender() override;

		void SetupDockSpace();

	private:
		std::shared_ptr<Framebuffer> m_Framebuffer;
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<Shader> m_GridShader;
		std::shared_ptr<VertexArray> m_GridVertexArray;
		std::shared_ptr<VertexArray> m_VertexArray;
		
		std::vector<glm::vec3> positions;
		EditorCamera* m_EditorCamera;
	};
}