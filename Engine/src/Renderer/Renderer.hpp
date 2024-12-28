#pragma once
#include "Crestaph.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/RendererAPI.hpp"
#include "Renderer/RendererCommand.hpp"

namespace Cresta {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(Camera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static std::unique_ptr<SceneData> s_SceneData;
	};
}
