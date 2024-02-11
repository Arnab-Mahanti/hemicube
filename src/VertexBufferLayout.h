#pragma once

#include <vector>
#include "Utility.h"

struct VertexBufferElement
{
    uint32_t type;
    int count;
    bool normalized;

    static uint32_t GetSizeOfType(uint32_t type)
    {
        switch (type)
        {
        case GL_FLOAT:
            return sizeof(GLfloat);
        case GL_UNSIGNED_INT:
            return sizeof(GLuint);
        case GL_UNSIGNED_BYTE:
            return sizeof(GLubyte);

        default:
            GL_ASSERT(false);
            return 0;
        }
    }
};

class VertexBufferLayout
{
private:
    int m_Stride;
    std::vector<VertexBufferElement> m_Elements;

public:
    VertexBufferLayout() : m_Stride(0) {}
    ~VertexBufferLayout(){};

    template <typename T>
    void Push(int count, bool normalized = GL_FALSE)
    {
        static_assert(false);
    }

    template <>
    void Push<float>(int count, bool normalized)
    {
        m_Elements.push_back({GL_FLOAT, count, normalized});
        m_Stride += VertexBufferElement ::GetSizeOfType(GL_FLOAT) * count;
    }

    template <>
    void Push<uint32_t>(int count, bool normalized)
    {
        m_Elements.push_back({GL_UNSIGNED_INT, count, normalized});
        m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
    }

    template <>
    void Push<unsigned char>(int count, bool normalized)
    {
        m_Elements.push_back({GL_UNSIGNED_BYTE, count, true});
        m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
    }

    inline auto GetElements() const { return m_Elements; }
    inline uint32_t GetStride() const { return m_Stride; }
};
