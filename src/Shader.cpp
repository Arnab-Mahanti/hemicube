#include "Shader.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string &vfilepath, const std::string &ffilepath)
    : m_RendererID(0), m_vfilepath(vfilepath), m_ffilepath(ffilepath)
{
    std::ifstream vertexShaderFile(m_vfilepath);
    std::ostringstream vertexStream;
    vertexStream << vertexShaderFile.rdbuf();
    std::ifstream fragmentShaderFile(m_ffilepath);
    std::ostringstream fragmentStream;
    fragmentStream << fragmentShaderFile.rdbuf();

    auto vertexShader = vertexStream.str();
    auto fragmentShader = fragmentStream.str();

    CompileShader(vertexShader, fragmentShader);
}

Shader::~Shader()
{
    GL_Call(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GL_Call(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GL_Call(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
    int location;
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        location = m_UniformLocationCache[name];

    else
    {
        location = glGetUniformLocation(m_RendererID, name.c_str());
        m_UniformLocationCache[name] = location;
    }
    if (location == -1)
    {
        std::cout << "[WARNING] : Uniform " << name << " could not be found!" << std::endl;
    }
    GL_Call(glUniform4f(location, v0, v1, v2, v3));
}

void Shader::SetUniform1i(const std::string &name, int v0)
{
    int location;
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        location = m_UniformLocationCache[name];

    else
    {
        location = glGetUniformLocation(m_RendererID, name.c_str());
        m_UniformLocationCache[name] = location;
    }
    if (location == -1)
    {
        std::cout << "[WARNING] : Uniform " << name << " could not be found!" << std::endl;
    }
    GL_Call(glUniform1i(location, v0));
}

void Shader::CompileShader(const std::string &vertexShader, const std::string &fragmentShader)
{

    m_RendererID = glCreateProgram();

    auto vs = glCreateShader(GL_VERTEX_SHADER);
    auto fs = glCreateShader(GL_FRAGMENT_SHADER);

    auto vsrc = vertexShader.c_str();
    auto fsrc = fragmentShader.c_str();

    int result;

    glShaderSource(vs, 1, &vsrc, nullptr);
    glShaderSource(fs, 1, &fsrc, nullptr);

    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(vs, length, &length, message);
        std::cout << "[ERROR]: Vertex Shader: " << message << "\n";
        glDeleteShader(vs);
    }

    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(fs, length, &length, message);
        std::cout << "[ERROR]: Fragment Shader: " << message << "\n";
        glDeleteShader(fs);
    }

    glAttachShader(m_RendererID, vs);
    glAttachShader(m_RendererID, fs);

    glLinkProgram(m_RendererID);
    glValidateProgram(m_RendererID);
    glDeleteShader(vs);
    glDeleteShader(fs);
}