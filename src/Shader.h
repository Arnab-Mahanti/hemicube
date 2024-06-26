#pragma once

#include "Utility.h"
#include "glm/glm.hpp"
#include <string>
#include <unordered_map>

class Shader
{
private:
    uint32_t m_RendererID;
    std::string m_vfilepath;
    std::string m_ffilepath;
    std::string m_gfilepath;
    std::unordered_map<std::string, int> m_UniformLocationCache;
    void Shader::CompileShader(const std::string &vertexShader, const std::string &fragmentShader, const std::string& geometryShader);

public:
    Shader(const std::string &vfilepath, const std::string &ffilepath, const std::string& gfilepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniform4f(const std::string &name, float v0, float v1, float v3, float v4);
    void SetUniform1i(const std::string &name, int v0);
    void SetUniformMat4f(const std::string &name, const glm::mat4 &matrix);
};
