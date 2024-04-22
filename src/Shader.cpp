#include "Shader.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string &vfilepath, const std::string &ffilepath, const std::string &gfilepath)
    : m_RendererID(0), m_vfilepath(vfilepath), m_ffilepath(ffilepath), m_gfilepath(gfilepath)
{
    std::ifstream vertexShaderFile(m_vfilepath);
    std::ostringstream vertexStream;
    vertexStream << vertexShaderFile.rdbuf();
    std::ifstream fragmentShaderFile(m_ffilepath);
    std::ostringstream fragmentStream;
    fragmentStream << fragmentShaderFile.rdbuf();
    std::ifstream geometryShaderFile(m_gfilepath);
    std::ostringstream geometryStream;
    geometryStream << geometryShaderFile.rdbuf();

    auto vertexShader = vertexStream.str();
    auto fragmentShader = fragmentStream.str();
    auto geometryShader = geometryStream.str();

    CompileShader(vertexShader, fragmentShader ,geometryShader);
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

void Shader::SetUniformMat4f(const std::string &name, const glm::mat4 &matrix)
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
    GL_Call(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]));
}

void Shader::CompileShader(const std::string &vertexShader, const std::string &fragmentShader, const std::string& geometryShader)
{

    m_RendererID = glCreateProgram();

    auto vs = glCreateShader(GL_VERTEX_SHADER);
    auto fs = glCreateShader(GL_FRAGMENT_SHADER);
    auto gs = glCreateShader(GL_GEOMETRY_SHADER);

    auto vsrc = vertexShader.c_str();
    auto fsrc = fragmentShader.c_str();
    auto gsrc = geometryShader.c_str();

    int result;

    glShaderSource(vs, 1, &vsrc, nullptr);
    glShaderSource(fs, 1, &fsrc, nullptr);
    glShaderSource(gs, 1, &gsrc, nullptr);

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

    glCompileShader(gs);
    glGetShaderiv(gs, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(gs, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(gs, length, &length, message);
        std::cout << "[ERROR]: Geometry Shader: " << message << "\n";
        glDeleteShader(gs);
    }

    glAttachShader(m_RendererID, vs);
    glAttachShader(m_RendererID, fs);
    glAttachShader(m_RendererID, gs);

    glLinkProgram(m_RendererID);
    glValidateProgram(m_RendererID);
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteShader(gs);
}