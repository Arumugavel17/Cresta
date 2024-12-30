#pragma once
#include "Renderer/RendererCommand.hpp"

namespace Cresta
{
	std::shared_ptr<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}