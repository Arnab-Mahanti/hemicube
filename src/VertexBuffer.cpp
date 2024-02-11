#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void *data, uint32_t size)
{
    GL_Call(glGenBuffers(1, &m_RendererID));
    GL_Call(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GL_Call(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GL_Call(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    GL_Call(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GL_Call(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
