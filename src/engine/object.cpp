#include "engine/object.h"
#include "engine/shader.h"
#include "utils/imageBMP.h"
#include "utils/model.h"



Object3D::Object3D(Model* model, imageBMP* texture) {
    this->model = model;
    this->texture = texture;
    count++;
    id = count;
}
Object3D::Object3D(Model* model) {
    this->model = model;
    count++;
    id = count;
}
Object3D::Object3D(const char* modelpath, const char* texturepath) {
    this->model = new Model(modelpath);
    this->texture = new imageBMP(texturepath);
    this->shader = new Shader("./shaders/block.vert", "./shaders/block.frag");
    count++;
    id = count;
}
Object3D::Object3D(const char* modelpath) {
    this->model = new Model(modelpath);
    this->shader = new Shader("./shaders/block.vert", "./shaders/no-texture.frag");

    count++;
    id = count;
}
bool Object3D::operator<(const Object3D& other) const {
    return id < other.id;
}
bool Object3D::operator==(const Object3D& other) const {
    return id == other.id;
}

void Object3D::render(const ShaderOptions& options) const {

    const Object3D obj = (*this);

    this->shader->drawObjectInstaced(obj, options);
};