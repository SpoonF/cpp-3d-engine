#pragma once

#include <vector>
#include <map>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

class Model {
    public:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    Model() = default;
    Model(const char* filename);

    static std::map<int, Model*> instances;
    static Model* getInstance(const char* filename, int id);
    static Model* getInstance(int id);
};
