#include "Crestaph.hpp"
#include "Platform/OpenGL/OpenGLTexture.hpp"
#include <stb_image.h>

namespace Cresta 
{
	namespace Utils {

		static GLenum CrestaImageFormatToGLDataFormat(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RGB8:  return GL_RGB;
			case ImageFormat::RGBA8: return GL_RGBA;
			}

			CRESTA_ASSERT(true);
			return 0;
		}

		static GLenum CrestaImageFormatToGLInternalFormat(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RGB8:  return GL_RGB8;
			case ImageFormat::RGBA8: return GL_RGBA8;
			}

			CRESTA_ASSERT(true);
			return 0;
		}

	}

	OpenGLTexture2D::OpenGLTexture2D(int height, int width, unsigned char* data)
	{
		
	}

	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification)
		: m_Specification(specification), m_Width(m_Specification.Width), m_Height(m_Specification.Height)
	{
		m_InternalFormat = Utils::CrestaImageFormatToGLInternalFormat(m_Specification.Format);
		m_DataFormat = Utils::CrestaImageFormatToGLDataFormat(m_Specification.Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		CreateHandle();
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path, bool flipTexture)
	{
		m_Path = path;
		CRESTA_CORE_INFO("{0} Texture Created ", path);
		int width, height, channels;
		stbi_set_flip_vertically_on_load(flipTexture);
		stbi_uc* data = nullptr;
		{
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}

		if (data)
		{
			m_IsLoaded = true;

			m_Width = width;
			m_Height = height;

			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 1)
			{
				internalFormat = GL_R8;
				dataFormat = GL_RED;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}
			else if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}

			m_InternalFormat = internalFormat;
			m_DataFormat = dataFormat;

			CRESTA_ASSERT(!(m_InternalFormat & m_DataFormat), "Format not supported!");
			
			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
		
		CreateHandle();
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	uint64_t OpenGLTexture2D::CreateHandle()
	{
		m_Handle = glGetTextureHandleARB(m_RendererID);
		glMakeTextureHandleResidentARB(m_Handle);
		return m_Handle;
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		CRESTA_ASSERT(size != m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}
