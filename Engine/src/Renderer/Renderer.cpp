#include "Renderer.hpp"
#include "Crestaph.hpp"

namespace Cresta 
{
	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();
	Ref<VertexArray> Renderer::s_SpriteVertexArray = nullptr;
	Ref<Shader> Renderer::s_SpriteShader = nullptr;
	Ref<Texture2D> Renderer::s_NoTexture = nullptr;

	void Renderer::Init()
	{
		s_NoTexture = Texture2D::Create("assets/Textures/images.jpg");
		s_SpriteShader = Shader::Create("assets/shaders/SpriteShader.glsl");
		s_SpriteVertexArray = VertexArray::Create();

		float vertices[] = {
			// Positions        // Texture Coords
			-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // Bottom-left
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // Bottom-right
			 0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // Top-right
			-0.5f,  0.5f, 0.0f,  0.0f, 1.0f  // Top-left
		};

		uint32_t indices[] = {
			0, 1, 2, // First triangle
			0, 2, 3  // Second triangle
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

	void Renderer::DrawSprite(const glm::vec3& color, const Ref<Texture2D>& texture, const glm::mat4& transform, float Mixfactor)
	{
		s_SpriteShader->Bind();
		s_SpriteShader->SetMat4("u_ProjectionView", s_SceneData->ViewProjectionMatrix);
		s_SpriteShader->SetVec3("u_CameraPosition", s_SceneData->CamerPosition);
		s_SpriteShader->SetMat4("u_Model", transform);
		s_SpriteShader->SetFloat("u_Mixfactor", Mixfactor);
		if (texture)
		{
			texture->Bind();
		}
		else
		{
			s_NoTexture->Bind(); 
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
