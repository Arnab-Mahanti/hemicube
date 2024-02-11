#pragma once

#include"Utility.h"
#include <string>
#include <unordered_map>

class Shader
{
private:
    uint32_t m_RendererID;
    std::string m_vfilepath;
    std::string m_ffilepath;
    std::unordered_map<std::string,uint32_t> m_UniformLocationCache;
    void Shader::CompileShader(const std::string &vertexShader, const std::string &fragmentShader);

public:
    Shader(const std::string &vfilepath, const std::string &ffilepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniform4f(const std::string &name, float v0, float v1, float v3, float v4);
};
