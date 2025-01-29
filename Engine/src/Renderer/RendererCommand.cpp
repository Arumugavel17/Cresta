#pragma once
#include "Renderer/RendererCommand.hpp"

namespace Cresta
{
	Ref<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}