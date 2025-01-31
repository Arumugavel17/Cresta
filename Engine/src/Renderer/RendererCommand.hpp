#pragma once
#include "Renderer/RendererAPI.hpp"

namespace Cresta 
{
	class RenderCommand
	{
	public:
		static void Init()
		{
			sm_RendererAPI->Init();
		}

		static void Enable()
		{
			sm_RendererAPI->Enable();
		}


		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			sm_RendererAPI->SetViewport(x, y, width, height);
		}

		static void SetClearColor(const glm::vec4& color)
		{
			sm_RendererAPI->SetClearColor(color);
		}

		static void Clear()
		{
			sm_RendererAPI->Clear();
		}

		static void DrawTriangles(unsigned int IndicesCount) 
		{
			sm_RendererAPI->DrawTriangles(IndicesCount);
		}

		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			sm_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}

		static void DrawLines(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount)
		{
			sm_RendererAPI->DrawLines(vertexArray, vertexCount);
		}

		static void SetLineWidth(float width)
		{
			sm_RendererAPI->SetLineWidth(width);
		}

		static float GetTime() 
		{
			return sm_RendererAPI->GetTime();
		}

	private:
		static Ref<RendererAPI> sm_RendererAPI;
	};

}
