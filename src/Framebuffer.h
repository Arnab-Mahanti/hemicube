#include "Utility.h"

#include "glad/glad.h"

struct FramebufferSpecification
{
    uint32_t width, height;
    uint32_t samples = 1;
};

class Framebuffer
{
private:
    uint32_t m_RendererID;
    uint32_t m_ColorAttachment, m_DepthAttachment;
    FramebufferSpecification m_Specification;

public:
    Framebuffer(const FramebufferSpecification &spec);
    ~Framebuffer();

    void Invalidate();
    void Bind();
    void Unbind();
    uint32_t GetColorAttachmentRendererID() const;
    void Blit() const;
};
