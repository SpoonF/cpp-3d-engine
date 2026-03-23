#include "object.h"


Object3D::Object3D(){};

Object3D::Object3D(Model* model, imageBMP* _texture, Shader* shader) {
    this->model = model;
    this->texture = texture;
    this->shader = shader;
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

ShaderOptions* Object3D::getOptions() {
    return this->options;
};
void Object3D::draw() {

    this->shader->drawObjectInstaced((*this));
};