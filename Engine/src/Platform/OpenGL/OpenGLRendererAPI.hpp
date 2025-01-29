#pragma once
#include "Renderer/RendererAPI.hpp"
#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace Cresta {
	class OpenGLRenderAPI : public RendererAPI {
	public:
		~OpenGLRenderAPI() = default;

		void Init() override;
		void Enable() override;
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;
		float GetTime() override;

		void DrawTriangles(unsigned int IndicesCount) override;
		void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount ) override;
		void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;

		void SetLineWidth(float width) override;
	};
}