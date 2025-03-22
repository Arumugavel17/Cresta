#pragma once

#include "Renderer/Texture.hpp"
#include <glad/glad.h>

namespace Cresta
{
	class OpenGLCubeMap : public CubeMap
	{
	public:
		OpenGLCubeMap(const std::vector<std::string> texture, bool flipTexture = true);

		virtual const TextureSpecification& GetSpecification() const override 
		{ 
			return m_Specification;
		}

		virtual uint64_t GetTextureHandle() const override { return m_Handle; };
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual uint64_t CreateHandle() override;

		virtual const std::string& GetPath() const override { return m_Path; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool IsLoaded() const override { return m_IsLoaded; }

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == other.GetRendererID();
		}

	private:
		TextureSpecification m_Specification;

		bool m_IsLoaded = false;

		uint64_t m_Handle;
		int32_t m_Width, m_Height;
		uint32_t m_RendererID;

		GLenum m_InternalFormat, m_DataFormat;
		std::string m_Path;
	};

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(int height, int width, unsigned char* data);
		OpenGLTexture2D(const TextureSpecification& specification);
		OpenGLTexture2D(const std::string& path, bool slipTexture );
		virtual ~OpenGLTexture2D();

		virtual const TextureSpecification& GetSpecification() const override { return m_Specification; }

		virtual uint64_t GetTextureHandle() const override { return m_Handle; };
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual uint64_t CreateHandle() override;

		virtual const std::string& GetPath() const override { return m_Path; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool IsLoaded() const override { return m_IsLoaded; }

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == other.GetRendererID();
		}

	private:
		TextureSpecification m_Specification;
		
		bool m_IsLoaded = false;
		
		uint64_t m_Handle;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		
		GLenum m_InternalFormat, m_DataFormat;
		std::string m_Path;
	};
}
