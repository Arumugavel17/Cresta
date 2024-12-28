#include "Crestaph.hpp"
#include "Renderer/RendererAPI.hpp"
#include "Platform/OpenGL/OpenGLRendererAPI.hpp"

namespace Cresta {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	std::unique_ptr<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPI::API::None:    CRESTA_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_unique<OpenGLRenderAPI>();
		}

		CRESTA_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}