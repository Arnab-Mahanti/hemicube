#pragma once
#include <glad/glad.h>
#include <iostream>

#define GL_ASSERT(x) \
    if (!(x))        \
        __debugbreak();
#define GL_Call(x)  \
    GLClearError(); \
    x;              \
    GL_ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char *function, const char *file, int line);