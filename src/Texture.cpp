#include "Texture.h"
#include "stb/stb_image.h"

Texture::Texture(const std::string &filepath)
    : m_RendererID(0), m_FilePath(filepath), m_LocalBuffer(nullptr), m_Height(0), m_Width(0), m_BPP(0)
{
    stbi_set_flip_vertically_on_load(1);

    m_LocalBuffer = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    GL_Call(glGenTextures(1, &m_RendererID));
    GL_Call(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GL_Call(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
    GL_Call(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture()
{
    if (m_LocalBuffer)
        stbi_image_free(m_LocalBuffer);

    glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(uint32_t slot) const
{
    GL_Call(glActiveTexture(GL_TEXTURE0 + slot));
    GL_Call(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
    GL_Call(glBindTexture(GL_TEXTURE_2D, 0));
}
