#pragma once
#include "Renderer/RendererAPI.hpp"
#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace Cresta 
{
	class OpenGLRenderAPI : public RendererAPI 
	{
	public:
		~OpenGLRenderAPI() = default;

		virtual float GetTime() override;

		virtual void Init() override;
		virtual void Enable() override;
		virtual void Clear() override;

		virtual void DrawTriangles(unsigned int IndicesCount) override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount ) override;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;

		virtual void SetLineWidth(float width) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		
		virtual void SetDrawModeToWireFrame() override;
		virtual void SetDrawModeToNormal() override;
		
	};
}