#include "Crestaph.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Renderer2D.hpp"

namespace Cresta {

	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{

	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(Camera& camera)
	{
		s_SceneData->CamerPosition = camera.GetPosition();
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
		Flush();
	}

	void Renderer::Flush()
	{

	}

	void Renderer::DrawTriangle(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform, unsigned int IndicesCount)
	{
		shader->Bind();
		shader->SetMat4("u_ProjectionView", s_SceneData->ViewProjectionMatrix);
		shader->SetVec3("u_CameraPosition", s_SceneData->CamerPosition);
		shader->SetMat4("u_Model", transform);
		vertexArray->Bind();
		RenderCommand::DrawTriangles(IndicesCount);
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ProjectionView", s_SceneData->ViewProjectionMatrix);
		shader->SetVec3("u_CameraPosition", s_SceneData->CamerPosition);
		shader->SetMat4("u_Model", transform);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}