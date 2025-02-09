#include "RendererCommand.hpp"

namespace Cresta
{
	Ref<RendererAPI> RenderCommand::sm_RendererAPI = RendererAPI::Create();

	float RenderCommand::GetTime()
	{
		return sm_RendererAPI->GetTime();
	}

	void RenderCommand::Init()
	{
		sm_RendererAPI->Init();
	}

	void RenderCommand::Clear()
	{
		sm_RendererAPI->Clear();
	}

	void RenderCommand::Enable()
	{
		sm_RendererAPI->Enable();
	}

	void RenderCommand::DrawTriangles(unsigned int IndicesCount)
	{
		sm_RendererAPI->DrawTriangles(IndicesCount);
	}

	void RenderCommand::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount)
	{
		sm_RendererAPI->DrawIndexed(vertexArray, indexCount);
	}

	void RenderCommand::DrawLines(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		sm_RendererAPI->DrawLines(vertexArray, vertexCount);
	}

	void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		sm_RendererAPI->SetViewport(x, y, width, height);
	}

	void RenderCommand::SetClearColor(const glm::vec4& color)
	{
		sm_RendererAPI->SetClearColor(color);
	}

	void RenderCommand::SetLineWidth(float width)
	{
		sm_RendererAPI->SetLineWidth(width);
	}

	void Cresta::RenderCommand::SetDrawModeToWireFrame()
	{
		sm_RendererAPI->SetDrawModeToWireFrame();
	}

	void Cresta::RenderCommand::SetDrawModeToNormal()
	{
		sm_RendererAPI->SetDrawModeToNormal();
	}
}