#include "Platform/OpenGL/OpenGLFramebuffer.hpp"
#include "Platform/OpenGL/Utils.hpp"
#include "glad/glad.h"

namespace Cresta {
	static const uint32_t s_MaxFramebufferSize = 8192;

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec) : m_Specification(spec)
    {
        for (auto spec : m_Specification.Attachments.Attachments)
        {
            if (!Utils::IsDepthFormat(spec.TextureFormat))
                m_ColorAttachmentSpecifications.emplace_back(spec);
            else
                m_DepthAttachmentSpecification = spec;
        }

        Recreate();
    }

    void OpenGLFramebuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, m_Specification.Width, m_Specification.Height);
    }

    void OpenGLFramebuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

	void OpenGLFramebuffer::Recreate()
	{

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
        CRESTA_ASSERT(attachmentIndex < m_ColorAttachments.size());

        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        int pixelData;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
        return pixelData;
    }

    void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
    {
        CRESTA_ASSERT(attachmentIndex < m_ColorAttachments.size());

        auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];
        glClearTexImage(m_ColorAttachments[attachmentIndex], 0,
        Utils::HazelFBTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
    }

    uint32_t OpenGLFramebuffer::GetColorAttachmentRendererID(uint32_t index) const
    {
        return 0;
    }
}