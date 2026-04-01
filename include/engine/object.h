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
    static std::vector<Object3D&> instances;
    Object3D() = default;
    Object3D(Model* _model, imageBMP* _texture, std::unique_ptr<Shader> shader);
    Object3D(Model* _model, std::unique_ptr<Shader> shader);
    Object3D(const char* modelpath, const char* texturepath, std::unique_ptr<Shader> shader);
    Object3D(const char* modelpath, const char* texturepath);
    Object3D(const char* modelpath);
    bool operator<(const Object3D& other) const;
    bool operator==(const Object3D& other) const;

    void render(const ShaderOptions& options) const;
};
