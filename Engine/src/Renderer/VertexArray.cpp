#include "VertexArray.hpp"
#include "Platform/OpenGL/OpenGLVertexArray.hpp"
#include "Renderer/Renderer.hpp"

namespace Cresta {
    std ::shared_ptr<VertexArray> Cresta::VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:    CRESTA_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexArray>();
        }

        CRESTA_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
