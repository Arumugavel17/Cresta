#include "Renderer/Framebuffer.hpp"
#include "Renderer/Renderer.hpp"
#include "Platform/OpenGL/OpenGLFramebuffer.hpp"

namespace Cresta {

	std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CRESTA_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLFramebuffer>(spec);
		}

		CRESTA_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}

