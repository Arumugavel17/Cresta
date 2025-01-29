#pragma once

#include "Crestaph.hpp"
#include "Renderer/UniformBuffer.hpp"

namespace Cresta
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(uint32_t size, uint32_t binding);
		OpenGLUniformBuffer(uint32_t size, uint32_t binding,const void* Data);
		virtual ~OpenGLUniformBuffer();
		
		virtual void Bind() override;
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
	private:
		uint32_t m_Binding;
		uint32_t m_RendererID = 0;
	};
}
