#ifndef OBJECT_H
#define OBJECT_H

#include "utils/imageBMP.cpp"
#include "utils/model.cpp"

class Object3D {
public:
    int id;
    static inline int count = 0;
    Model* model;
    imageBMP* texture;
    Object3D();
    Object3D(Model* _model, imageBMP* _texture);
    Object3D(const char* modelpath, const char* texturepath);
    Object3D(const char* modelpath);
    bool operator<(const Object3D& other) const;
    bool operator==(const Object3D& other) const;
};

#endif