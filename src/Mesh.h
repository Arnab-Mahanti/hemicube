#pragma once

#include <memory>
#include <string>

template <typename T>
class Mesh
{
private:
    std::shared_ptr<T> vertices;
    std::shared_ptr<T> indices;
public:
    Mesh(const std::string& filepath);
    ~Mesh();
};
