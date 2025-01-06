#include "Crestaph.hpp"
#include "Renderer/UniformBuffer.hpp"
#include "Renderer/Renderer.hpp"
#include "Platform/OpenGL/OpenGLUniformBuffer.hpp"

namespace Cresta
{
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CRESTA_ASSERT(true, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLUniformBuffer>(size, binding);
		}

		CRESTA_ASSERT(true, "Unknown RendererAPI!");
		return nullptr;
	}

}
