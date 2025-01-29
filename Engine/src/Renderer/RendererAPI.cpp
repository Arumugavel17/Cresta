#include "Crestaph.hpp"
#include "Renderer/RendererAPI.hpp"
#include "Platform/OpenGL/OpenGLRendererAPI.hpp"

namespace Cresta {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPI::API::None:    CRESTA_ASSERT(true, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLRenderAPI>();
		}

		CRESTA_ASSERT(true, "Unknown RendererAPI!");
		return nullptr;
	}

}