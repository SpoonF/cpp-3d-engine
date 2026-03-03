#include "object.h"


Object3D::Object3D(){};

Object3D::Object3D(Model* _model, imageBMP* _texture) {
    model = _model;
    texture = _texture;
    count++;
    id = count;
}
Object3D::Object3D(const char* modelpath, const char* texturepath) {
    model = new Model(modelpath);
    texture = new imageBMP(texturepath);
    count++;
    id = count;
}
Object3D::Object3D(const char* modelpath) {
    model = new Model(modelpath);
    count++;
    id = count;
}
bool Object3D::operator<(const Object3D& other) const {
    return id < other.id;
}
bool Object3D::operator==(const Object3D& other) const {
    return id == other.id;
}