#include "Crestaph.hpp"
#include "Platform/OpenGL/OpenGLTexture.hpp"
#include <stb_image.h>

namespace Cresta 
{
	namespace Utils 
	{
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

	OpenGLCubeMap::OpenGLCubeMap(const std::vector<std::string> texture, bool flipTexture )
	{
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		int nrChannels;
		for (int32_t i = 0; i < texture.size(); i++)
		{
			unsigned char* data = stbi_load(texture[i].c_str(), &m_Width, &m_Height, &nrChannels, 0);
			if (nrChannels == 4)
			{
				m_DataFormat = GL_RGBA;
			}
			else if (nrChannels == 3)
			{
				m_DataFormat = GL_RGB;
			}
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, m_DataFormat, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap tex failed to load at path: " << texture[i] << std::endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	uint64_t OpenGLCubeMap::CreateHandle()
	{
		CRESTA_ASSERT("Cube Map does not have bindless texture handle");
		return 0;
	}

	void OpenGLCubeMap::SetData(void* data, uint32_t size)
	{
		CRESTA_ASSERT("Cannot set data of a cubemap");
		return;
	}

	void OpenGLCubeMap::Bind(uint32_t slot) const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
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
