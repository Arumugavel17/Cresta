#pragma once
#include "Crestaph.hpp"
#include "Renderer/FrameBuffer.hpp"

namespace Cresta {

    class OpenGLFramebuffer : public Framebuffer {
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		~OpenGLFramebuffer() = default;

		void Bind() override;
		void Unbind() override;
		void Recreate();
		void Resize(uint32_t width, uint32_t height) override;
		int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		void ClearAttachment(uint32_t attachmentIndex, int value) override;

		uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override;

		const FramebufferSpecification& GetSpecification() const override { return m_Specification; };
	private:
		FramebufferSpecification m_Specification;
	};
}