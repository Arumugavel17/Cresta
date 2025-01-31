#pragma once
#include "Renderer/RendererCommand.hpp"

namespace Cresta
{
	Ref<RendererAPI> RenderCommand::sm_RendererAPI = RendererAPI::Create();
}