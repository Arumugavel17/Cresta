#pragma once
#include "Crestaph.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/RendererAPI.hpp"
#include "Renderer/RendererCommand.hpp"
#include "Renderer/Model.hpp"

namespace Cresta 
{
	
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(Camera& camera);
		
		static void EndScene();
		static void Flush();
		static void DrawSprite(const glm::vec4& color, const Ref<Texture2D>& texture, const glm::mat4& transform, float Mixfactor = 0);
		static void DrawTriangle(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform, unsigned int IndicesCount);
		static void DrawIndexed(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
		static void DrawIndexed(const Ref<Shader>& shader, const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer);

		static void DrawGizmoTriangle(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform, unsigned int IndicesCount);
		static void DrawGizmoIndexed(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
		static void DrawGizmoIndexed(const Ref<Shader>& shader, const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer);

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::vec3 CamerPosition;
			glm::mat4 ViewProjectionMatrix;
		};


		static Ref<Shader> sm_SpriteShader;
		static Ref<VertexArray> sm_SpriteVertexArray;
		static Scope<SceneData> sm_SceneData;
	};
}
