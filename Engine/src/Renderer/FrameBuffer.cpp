#include "Renderer/Framebuffer.hpp"
#include "Renderer/Renderer.hpp"
#include "Platform/OpenGL/OpenGLFramebuffer.hpp"

namespace Cresta {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CRESTA_ASSERT(true, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLFramebuffer>(spec);
		}

		CRESTA_ASSERT(true, "Unknown RendererAPI!");
		return nullptr;
	}

}

