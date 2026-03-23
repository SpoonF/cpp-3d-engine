#ifndef OBJECT_H
#define OBJECT_H

#include "utils/imageBMP.cpp"
#include "utils/model.cpp"
#include "shader.h"

class Object3D {
public:
    int id;
    static inline int count = 0;
    Model* model;
    imageBMP* texture;
    Shader* shader;
    ShaderOptions* options;
    static std::vector<Object3D&> instances;
    Object3D();
    Object3D(Model* _model, imageBMP* _texture, Shader* shader);
    Object3D(const char* modelpath, const char* texturepath);
    Object3D(const char* modelpath);
    bool operator<(const Object3D& other) const;
    bool operator==(const Object3D& other) const;

    ShaderOptions* getOptions();
    void draw();
};

#endif