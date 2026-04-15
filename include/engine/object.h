#pragma once

#include <vector>
#include <memory>

class Model;
class imageBMP;
class Shader;
class ShaderOptions;

class Object3D {
public:
    int id;
    static inline int count = 0;
    Model* model;
    imageBMP* texture;
    Shader* shader;
    // static std::vector<Object3D&> instances;
    glm::vec3 position;
    Object3D() = default;
    Object3D(Model* _model, imageBMP* _texture);
    Object3D(Model* _model);
    Object3D(const char* modelpath, const char* texturepath);
    Object3D(const char* modelpath);
    bool operator<(const Object3D& other) const;
    bool operator==(const Object3D& other) const;

    void render(const ShaderOptions& options) const;
};
