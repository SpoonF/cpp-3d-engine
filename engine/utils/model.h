#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

class Model {
    public:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    Model(const char* filename);
};

#endif