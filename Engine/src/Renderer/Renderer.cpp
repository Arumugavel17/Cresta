#include "Renderer.hpp"
#include "Renderer.hpp"
#include "Renderer.hpp"
#include "Renderer.hpp"
#include "Crestaph.hpp"

namespace Cresta 
{
	Scope<Renderer::SceneData> Renderer::sm_SceneData = CreateScope<Renderer::SceneData>();
	Ref<VertexArray> Renderer::sm_SpriteVertexArray = nullptr;
	Ref<Shader> Renderer::sm_SpriteShader = nullptr;

	void Renderer::Init()
	{
		sm_SpriteShader = Shader::Create("assets/shaders/SpriteShader.glsl");
		sm_SpriteVertexArray = VertexArray::Create();

		float vertices[] = {
			 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  // Top Right
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  // Bottom Right
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  // Bottom Left
			-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  // Top Left
		};

		uint32_t indices[] = 
		{
			0, 1, 3, // First triangle
			1, 2, 3  // Second triangle
		};

		Ref<VertexBuffer> VBO = VertexBuffer::Create(vertices, sizeof(vertices));
		Ref<IndexBuffer> IBO = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		VBO->SetLayout(
			{
				{ShaderDataType::FVec3, "aPos"},
				{ShaderDataType::FVec2, "aTexCoord"}
			}
		);

		sm_SpriteVertexArray->AddVertexBuffer(VBO);
		sm_SpriteVertexArray->SetIndexBuffer(IBO);

		//RenderCommand::Init();
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
		sm_SceneData->CamerPosition = camera.GetPosition();
		sm_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
		Flush();
	}

	void Renderer::Flush()
	{

	}

	void Renderer::DrawSprite(const glm::vec4& color, const Ref<Texture2D>& texture, const glm::mat4& transform, float Mixfactor)
	{
		sm_SpriteShader->Bind();
		sm_SpriteShader->SetMat4("u_ProjectionView", sm_SceneData->ViewProjectionMatrix);
		sm_SpriteShader->SetVec3("u_CameraPosition", sm_SceneData->CamerPosition);
		sm_SpriteShader->SetVec4("u_Color", color);
		sm_SpriteShader->SetMat4("u_Model", transform);
		sm_SpriteShader->SetFloat("u_Mixfactor", Mixfactor);
		sm_SpriteShader->SetInt("u_Texture", 0);
		sm_SpriteShader->SetInt("u_TextureSet", texture ? 1 : 0);
		if (texture)
		{
			texture->Bind();
		}
		sm_SpriteVertexArray->Bind();
		RenderCommand::DrawIndexed(sm_SpriteVertexArray);
	}

	void Renderer::DrawTriangle(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform, unsigned int IndicesCount)
	{
		shader->Bind();
		shader->SetMat4("u_ProjectionView", sm_SceneData->ViewProjectionMatrix);
		shader->SetVec3("u_CameraPosition", sm_SceneData->CamerPosition);
		shader->SetMat4("u_Model", transform);
		vertexArray->Bind();
		RenderCommand::DrawTriangles(IndicesCount);
	}

	void Renderer::DrawIndexed(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ProjectionView", sm_SceneData->ViewProjectionMatrix);
		shader->SetVec3("u_CameraPosition", sm_SceneData->CamerPosition);
		shader->SetMat4("u_Model", transform);
		shader->SetVec4("u_Color", glm::vec4(1.0f));
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::DrawIndexed(const Ref<Shader>& shader, const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer)
	{

	}

	//Gizmos////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Renderer::DrawGizmoTriangle(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform, unsigned int IndicesCount)
	{
		RenderCommand::SetDrawModeToWireFrame();
		shader->Bind();
		shader->SetMat4("u_ProjectionView", sm_SceneData->ViewProjectionMatrix);
		shader->SetVec3("u_CameraPosition", sm_SceneData->CamerPosition);
		shader->SetMat4("u_Model", transform);
		vertexArray->Bind();
		RenderCommand::DrawTriangles(IndicesCount);
		RenderCommand::SetDrawModeToNormal();

	}
	void Renderer::DrawGizmoIndexed(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		RenderCommand::SetDrawModeToWireFrame();
		shader->Bind();
		shader->SetMat4("u_ProjectionView", sm_SceneData->ViewProjectionMatrix);
		shader->SetVec3("u_CameraPosition", sm_SceneData->CamerPosition);
		shader->SetMat4("u_Model", transform);
		shader->SetVec4("u_Color", glm::vec4(1.0f));
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
		RenderCommand::SetDrawModeToNormal();
	}
	void Renderer::DrawGizmoIndexed(const Ref<Shader>& shader, const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer)
	{
		RenderCommand::SetDrawModeToWireFrame();
		RenderCommand::SetDrawModeToNormal();
	}
}
