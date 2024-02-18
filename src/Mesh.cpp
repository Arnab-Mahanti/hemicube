#include "Mesh.h"

#include <fstream>

template <typename T>
Mesh<T>::Mesh(const std::string &filepath)
{
    auto file = std::ifstream(filepath);
    std::string line;
    auto file_data = std::string(std::istreambuf_iterator{file}, {});
}

template <typename T>
Mesh<T>::~Mesh()
{
}