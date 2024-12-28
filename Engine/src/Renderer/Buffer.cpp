#include "Renderer/Buffer.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/RendererAPI.hpp"
#include "Platform/OpenGL/OpenGLBuffer.hpp"

namespace Cresta {

    std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:    CRESTA_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexBuffer>(size);
        }

        CRESTA_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:    CRESTA_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexBuffer>(vertices,size);
        }

        CRESTA_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:    CRESTA_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLIndexBuffer>(indices, count);
        }

        CRESTA_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}