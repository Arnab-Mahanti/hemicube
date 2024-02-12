#pragma once
#include "Utility.h"

class Texture
{
private:
    uint32_t m_RendererID;
    std::string m_FilePath;
    unsigned char *m_LocalBuffer;
    int m_Height, m_Width, m_BPP;

public:
    Texture(const std::string &filepath);
    ~Texture();

    void Bind(uint32_t slot = 0) const;
    void Unbind() const;
};
