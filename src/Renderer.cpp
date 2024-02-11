#include "Renderer.h"

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const
{
    va.Bind();
    ib.Bind();
    shader.Bind();
    GL_Call(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
    GL_Call(glClear(GL_COLOR_BUFFER_BIT));
}
