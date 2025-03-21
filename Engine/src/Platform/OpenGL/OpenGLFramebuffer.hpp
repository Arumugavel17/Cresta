#pragma once
#include "Crestaph.hpp"
#include "Renderer/FrameBuffer.hpp"

namespace Cresta 
{
    class OpenGLFramebuffer : public Framebuffer 
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		~OpenGLFramebuffer() = default;

		void Bind() override;
		void Unbind() override;
		void Recreate();
		void SetBufferSize(void* AttachmentArrya, int size) override;


		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override;
		virtual uint32_t GetDepthAttachement() const override;

		const FramebufferSpecification& GetSpecification() const override { return m_Specification; };

	private:
		FramebufferSpecification m_Specification;
	};
}