#include "engine/object.h"
#include "engine/shader.h"
#include "utils/imageBMP.h"
#include "utils/model.h"



Object3D::Object3D(Model* model, imageBMP* texture, std::unique_ptr<Shader> shader) {
    this->model = model;
    this->texture = texture;
    this->shader = shader.get();
    count++;
    id = count;
}
Object3D::Object3D(Model* model, std::unique_ptr<Shader> shader) {
    this->model = model;
    this->shader = shader.get();
    count++;
    id = count;
}
Object3D::Object3D(const char *modelpath, const char *texturepath, std::unique_ptr<Shader> shader)
{
    this->model = new Model(modelpath);
    this->texture = new imageBMP(texturepath);
    this->shader = new Shader("./shaders/shader.vert", "./shaders/shader.frag");
    count++;
    id = count;
}
Object3D::Object3D(const char* modelpath, const char* texturepath) {
    this->model = new Model(modelpath);
    this->texture = new imageBMP(texturepath);
    count++;
    id = count;
}
Object3D::Object3D(const char* modelpath) {
    this->model = new Model(modelpath);

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

    // ShaderOptions opt;

    this->shader->drawObjectInstaced(obj, options);
};