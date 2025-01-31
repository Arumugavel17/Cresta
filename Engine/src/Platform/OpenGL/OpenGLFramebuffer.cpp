#include "Platform/OpenGL/OpenGLFramebuffer.hpp"
#include "Platform/OpenGL/Utils.hpp"
#include "glad/glad.h"
#include "Renderer/RendererCommand.hpp"

namespace Cresta 
{
	static const uint32_t s_MaxFramebufferSize = 8192;

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec) : m_Specification(spec)
    {
        for (auto spec : m_Specification.Attachments.Attachments)
        {
			if (!Utils::IsDepthFormat(spec.TextureFormat))
			{
                p_ColorAttachmentSpecifications.emplace_back(spec);
			}
            else
			{
                p_DepthAttachmentSpecification = spec;
			}
        }

        Recreate();
    }

    void OpenGLFramebuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, p_RendererID);
        glViewport(0, 0, m_Specification.Width, m_Specification.Height);
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();
		RenderCommand::Enable();
    }

    void OpenGLFramebuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

	void OpenGLFramebuffer::Recreate()
	{
		if (p_RendererID)
		{
			glDeleteFramebuffers(1, &p_RendererID);
			glDeleteTextures(p_ColorAttachments.size(), p_ColorAttachments.data());
			glDeleteTextures(1, &p_DepthAttachment);

			p_ColorAttachments.clear();
			p_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &p_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, p_RendererID);

		bool multisample = m_Specification.Samples > 1;

		// Attachments
		if (p_ColorAttachmentSpecifications.size())
		{
			p_ColorAttachments.resize(p_ColorAttachmentSpecifications.size());
			Utils::CreateTextures(multisample, p_ColorAttachments.data(), p_ColorAttachments.size());

			for (size_t i = 0; i < p_ColorAttachments.size(); i++)
			{
				Utils::BindTexture(multisample, p_ColorAttachments[i]);
				switch (p_ColorAttachmentSpecifications[i].TextureFormat)
				{
				case FramebufferTextureFormat::RGBA8:
					Utils::AttachColorTexture(p_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
					break;
				case FramebufferTextureFormat::RGB:
					Utils::AttachColorTexture(p_ColorAttachments[i], m_Specification.Samples, GL_RGB, GL_RGB, m_Specification.Width, m_Specification.Height, i);
					break;
				case FramebufferTextureFormat::RED_INTEGER:
					Utils::AttachColorTexture(p_ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
					break;
				}
			}
		}

		if (p_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
			Utils::CreateTextures(multisample, &p_DepthAttachment, 1);
			Utils::BindTexture(multisample, p_DepthAttachment);
			switch (p_DepthAttachmentSpecification.TextureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(p_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
				break;
			}
		}

		if (p_ColorAttachments.size() > 1)
		{
			CRESTA_ASSERT(p_ColorAttachments.size() >= 4);
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(p_ColorAttachments.size(), buffers);
		}
		else if (p_ColorAttachments.empty())
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}

		CRESTA_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

    void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
    {
        if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
        {
            CRESTA_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
            return;
        }
        m_Specification.Width = width;
        m_Specification.Height = height;

        Recreate();
    }

    int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
    {
        CRESTA_ASSERT(attachmentIndex > p_ColorAttachments.size());

        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        int pixelData;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
        return pixelData;
    }

    void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
    {
        CRESTA_ASSERT(attachmentIndex > p_ColorAttachments.size());

        auto& spec = p_ColorAttachmentSpecifications[attachmentIndex];
        glClearTexImage(p_ColorAttachments[attachmentIndex], 0,
        Utils::CrestaFBTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
    }

    uint32_t OpenGLFramebuffer::GetColorAttachmentRendererID(uint32_t index) const
    {
		return p_ColorAttachments[index];
    }

	uint32_t OpenGLFramebuffer::GetDepthAttachement() const
	{
		return p_DepthAttachment;
	}
}