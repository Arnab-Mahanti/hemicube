#pragma once

#include <iostream>
#include <glad/glad.h>

#define GL_ASSERT(x) \
    if (!(x))        \
        __debugbreak();
#define GL_Call(x)  \
    GLClearError(); \
    x;              \
    GL_ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char *function, const char *file, int line);