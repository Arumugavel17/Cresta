#include "OpenGLRendererAPI.hpp"

namespace Cresta 
{
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         CRESTA_CORE_CRITICAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       CRESTA_CORE_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          CRESTA_CORE_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: CRESTA_CORE_TRACE(message); return;
		}

		CRESTA_ASSERT(true, "Unknown severity level!");
	}

	void OpenGLRenderAPI::Init()
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		Enable();
	}

	void OpenGLRenderAPI::Enable()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
	}
	void OpenGLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		CRESTA_CORE_INFO("x:{0} , y:{1}", width, height);
		glViewport(x, y, width, height);
	}

	void OpenGLRenderAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRenderAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	}
	float OpenGLRenderAPI::GetTime()
	{
		return glfwGetTime();
	}

	void OpenGLRenderAPI::DrawTriangles(unsigned int IndicesCount)
	{
		glDrawArrays(GL_TRIANGLES, 0, IndicesCount);
	}

	void OpenGLRenderAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRenderAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void OpenGLRenderAPI::SetLineWidth(float width)
	{
		glLineWidth(width);
	}
}