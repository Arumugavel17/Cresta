#pragma once

#include "Renderer/VertexArray.hpp"
#include <glm/glm.hpp>

namespace Cresta 
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
	public:
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void Enable() = 0;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual float GetTime() = 0;
		virtual void Clear() = 0;

		virtual void SetDrawModeToWireFrame() = 0;
		virtual void SetDrawModeToNormal() = 0;

		virtual void DrawTriangles(unsigned int IndicesCount) = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) = 0;

		virtual void SetLineWidth(float width) = 0;

		static API GetAPI() { return sm_API; }
		static Scope<RendererAPI> Create();
	private:
		static API sm_API;
	};

}
