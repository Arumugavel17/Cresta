#pragma once
#include "Crestaph.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/RendererAPI.hpp"
#include "Renderer/RendererCommand.hpp"
#include "Renderer/Model.hpp"

namespace Cresta {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(Camera& camera);
		
		static void EndScene();
		static void Flush();
		static void DrawModel(const Ref<Shader>& shader,Ref<Model> model, const glm::mat4& transform);
		static void DrawTriangle(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform, unsigned int IndicesCount);
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::vec3 CamerPosition;
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;
	};
}
