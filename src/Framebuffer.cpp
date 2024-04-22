#include "Framebuffer.h"

Framebuffer::Framebuffer(const FramebufferSpecification &spec)
    : m_Specification(spec)
{
    Invalidate();
}

Framebuffer::~Framebuffer()
{
    GL_Call(glDeleteTextures(1, &m_ColorAttachment));
    GL_Call(glDeleteTextures(1, &m_DepthAttachment));
    glDeleteFramebuffers(1, &m_RendererID);
}

void Framebuffer::Invalidate()
{
    GL_Call(glGenFramebuffers(1, &m_RendererID));
    GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

    GL_Call(glGenTextures(1, &m_ColorAttachment));
    GL_Call(glBindTexture(GL_TEXTURE_2D, m_ColorAttachment));
    GL_Call(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.width, m_Specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
    GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
    GL_Call(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0));

    GL_Call(glGenTextures(1, &m_DepthAttachment));
    GL_Call(glBindTexture(GL_TEXTURE_2D, m_DepthAttachment));
    GL_Call(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.width, m_Specification.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr));
    GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
    GL_Call(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0));

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "[OpenGL Error:] Framebuffer Incomplete\n";
    }
    GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::Bind()
{
    GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void Framebuffer::Unbind()
{
    GL_Call(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

uint32_t Framebuffer::GetColorAttachmentRendererID() const
{
    return m_ColorAttachment;
}

void Framebuffer::Blit() const
{
    GL_Call(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID));
    GL_Call(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
    GL_Call(glBlitFramebuffer(0, 0, m_Specification.width, m_Specification.height, 0, 0, m_Specification.width, m_Specification.height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST));
}
