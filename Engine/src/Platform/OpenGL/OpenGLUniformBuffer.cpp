#include "Crestaph.hpp"
#include "OpenGLUniformBuffer.hpp"
#include <glad/glad.h>

namespace Cresta {

	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding)
	{
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW); // TODO: investigate usage hint
		m_Binding = binding;
	}

	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding,const void* Data)
	{
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, size, Data, GL_DYNAMIC_DRAW); // TODO: investigate usage hint
		m_Binding = binding;
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}


	void OpenGLUniformBuffer::Bind()
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, m_Binding, m_RendererID);
	}

	void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		glNamedBufferSubData(m_RendererID, offset, size, data);
	}

}
