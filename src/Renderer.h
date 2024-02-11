#pragma once

#include <iostream>
#include <glad/glad.h>

#include "Utility.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
private:
    /* data */
public:
    void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;
    void Clear() const;
};
