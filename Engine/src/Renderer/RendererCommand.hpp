#pragma once
#include "Renderer/RendererAPI.hpp"

namespace Cresta 
{
	class RenderCommand
	{
	public:
		static float GetTime();

		static void Init();
		static void Clear();
		static void Enable();
		
		static void DrawLines(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount);
		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0);
		static void DrawTriangles(unsigned int IndicesCount);

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);		
		static void SetClearColor(const glm::vec4& color);
		static void SetLineWidth(float width);
		static void SetDrawModeToWireFrame();
		static void SetDrawModeToNormal();

	private:
		static Ref<RendererAPI> sm_RendererAPI;
	};

}
