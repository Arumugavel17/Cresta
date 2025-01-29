#include "Renderer.hpp"
#include "Crestaph.hpp"

namespace Cresta 
{
	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();
	Ref<VertexArray> Renderer::s_SpriteVertexArray = nullptr;
	Ref<Shader> Renderer::s_SpriteShader = nullptr;

	void Renderer::Init()
	{
		s_SpriteShader = Shader::Create("assets/shaders/SpriteShader.glsl");
		s_SpriteVertexArray = VertexArray::Create();

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

		s_SpriteVertexArray->AddVertexBuffer(VBO);
		s_SpriteVertexArray->SetIndexBuffer(IBO);

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

	void Renderer::DrawSprite(const glm::vec4& color, const Ref<Texture2D>& texture, const glm::mat4& transform, float Mixfactor)
	{
		s_SpriteShader->Bind();
		s_SpriteShader->SetMat4("u_ProjectionView", s_SceneData->ViewProjectionMatrix);
		s_SpriteShader->SetVec3("u_CameraPosition", s_SceneData->CamerPosition);
		s_SpriteShader->SetVec4("u_Color", color);
		s_SpriteShader->SetMat4("u_Model", transform);
		s_SpriteShader->SetFloat("u_Mixfactor", Mixfactor);
		s_SpriteShader->SetInt("u_Texture", 0);
		s_SpriteShader->SetInt("u_TextureSet", texture ? 1 : 0);
		if (texture)
		{
			texture->Bind();
		}
		s_SpriteVertexArray->Bind();
		RenderCommand::DrawIndexed(s_SpriteVertexArray);
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
		shader->SetVec4("u_Color", glm::vec4(1.0f));
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer)
	{

	}
}
