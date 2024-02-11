#include "VertexArray.h"
#include "VertexBufferLayout.h"


VertexArray::VertexArray()
{
    GL_Call(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
    GL_Call(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
    Bind();
    vb.Bind();
    const auto &elements = layout.GetElements();
    for (size_t i = 0; i < elements.size(); i++)
    {
        uint32_t offset = 0;
        auto &element = elements[i];
        GL_Call(glEnableVertexAttribArray(i));
        GL_Call(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void *)offset));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }

    for (auto &&i : elements)
    {
    }
}

void VertexArray::Bind() const
{
    GL_Call(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
    GL_Call(glBindVertexArray(0));
}
