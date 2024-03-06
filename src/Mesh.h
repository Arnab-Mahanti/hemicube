#pragma once

#include <memory>
#include <string>
#include <variant>
#include <fstream>
#include <unordered_map>
#include <chrono>

#include "glm/glm.hpp"

template <typename T = float>
class Mesh2D
{
private:
    std::vector<T> vertices;
    std::vector<uint32_t> indices;
    std::unordered_map<uint32_t, glm::vec3> NodeCollection;
    std::unordered_map<uint32_t, std::vector<uint32_t>> ElementCollection;

    void ReadFile(const std::string &filepath);

public:
    Mesh2D(const std::string &filepath);
    ~Mesh2D();
    const void *GetVertices() const;
    const uint32_t *GetIndices() const;
    size_t GetVertexSize() const;
    size_t GetIndexSize() const;
    size_t GetVertexCount() const;
    size_t GetIndexCount() const;
};

template <typename T>
Mesh2D<T>::Mesh2D(const std::string &filepath)
{
    ReadFile(filepath);

    uint32_t indexCount = 0;
    for (auto &&element : ElementCollection)
    {
        auto elementID = element.first;
        auto color = {((elementID >> 16) % 256) / 255.f,
                      ((elementID >> 8) % 256) / 255.f,
                      ((elementID >> 0) % 256) / 255.f,
                      1.f};

        // auto color = {
        //     (elementID & (0xFF << 16)) / 255.f,
        //     (elementID & (0xFF << 8)) / 255.f,
        //     (elementID & (0xFF << 0)) / 255.f,
        //     1.f};

        for (auto &&nodeID : element.second)
        {
            auto pos = NodeCollection[nodeID];
            vertices.insert(vertices.end(), {pos.x, pos.y, pos.z});
            vertices.insert(vertices.end(), color);
        }

        if (element.second.size() == 4)
        {
            indices.insert(indices.end(), {indexCount, indexCount + 1, indexCount + 2,
                                           indexCount + 2, indexCount + 3, indexCount});
            indexCount += 4;
        }
        else if (element.second.size() == 3)
        {
            indices.insert(indices.end(), {indexCount, indexCount + 1, indexCount + 2});
            indexCount += 3;
        }
    }
}

template <typename T>
Mesh2D<T>::~Mesh2D()
{
}

template <typename T>
inline const void *Mesh2D<T>::GetVertices() const
{
    return vertices.data();
}

template <typename T>
inline const uint32_t *Mesh2D<T>::GetIndices() const
{
    return indices.data();
}

template <typename T>
inline size_t Mesh2D<T>::GetVertexSize() const
{
    return sizeof(T) * vertices.size();
}

template <typename T>
inline size_t Mesh2D<T>::GetIndexSize() const
{
    return sizeof(std::iter_value_t<decltype(indices)>) * indices.size();
}

template <typename T>
inline size_t Mesh2D<T>::GetVertexCount() const
{
    return vertices.size();
}

template <typename T>
inline size_t Mesh2D<T>::GetIndexCount() const
{
    return indices.size();
}

template <typename T>
void Mesh2D<T>::ReadFile(const std::string &filepath)
{
    std::cout << "Reading Mesh File: " << filepath << "\n";
    auto start = std::chrono::high_resolution_clock::now();
    auto file = std::ifstream(filepath);
    std::string line;
    while (std::getline(file, line)) // Assumption: No leading whitespaces not required
    {
        if (line[0] == 'N')
        {
            auto offset_begin = line.find(',');
            auto offset_end = line.find(',', offset_begin + 1);
            auto s_ID = line.substr(offset_begin + 1, offset_end - offset_begin);
            uint32_t ID = std::atol(s_ID.c_str());

            offset_begin = offset_end;
            offset_end = line.find(',', offset_begin + 1);
            auto s_x = line.substr(offset_begin + 1, offset_end - offset_begin);
            auto x = std::atof(s_x.c_str());

            offset_begin = offset_end;
            offset_end = line.find(',', offset_begin + 1);
            auto s_y = line.substr(offset_begin + 1, offset_end - offset_begin);
            auto y = std::atof(line.substr(offset_begin + 1, offset_end - offset_begin).c_str());

            offset_begin = offset_end;
            offset_end = line.find(',', offset_begin + 1);
            auto s_z = line.substr(offset_begin + 1, offset_end - offset_begin);
            auto z = std::atof(line.substr(offset_begin + 1, offset_end - offset_begin).c_str());

            NodeCollection[ID] = glm::vec3(x, y, z);
        }

        // Assumption: All element entries start after Node entries

        else if (line[0] == 'E')
        {
            auto offset_begin = line.find(',');
            auto offset_end = line.find(',', offset_begin + 1);
            auto s_ID = line.substr(offset_begin + 1, offset_end - offset_begin);
            uint32_t ID = std::atol(s_ID.c_str());

            offset_begin = line.find("HTPLANE") + 7; // Length of HTPLANE
            offset_end = line.find(',', offset_begin + 1);
            auto s_numnodes = line.substr(offset_begin + 1, offset_end - offset_begin);
            auto numnodes = std::atoi(s_numnodes.c_str());

            std::vector<uint32_t> nodeids(numnodes, 0);
            for (size_t i = 0; i < numnodes; i++)
            {
                offset_begin = offset_end;
                offset_end = line.find(',', offset_begin + 1);
                auto s_nodeid = line.substr(offset_begin + 1, offset_end - offset_begin);
                nodeids[i] = std::atoi(s_nodeid.c_str());
            }
            ElementCollection[ID] = nodeids;
        }
    }
    std::cout << "Parsed file in: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count()
              << "ms\n";
}
