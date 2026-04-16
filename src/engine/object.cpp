#include "engine/object.h"
#include "engine/shader.h"
#include "utils/imageBMP.h"
#include "utils/model.h"



Object::Object(const Model& model,const imageBMP& texture, const glm::vec3& position, ObjectType type) {
    this->model = model;
    this->texture = texture;
    this->position = position;
    this->type = type;
    count++;
    id = count;
}
Object::Object(const Model& model, const glm::vec3& position, ObjectType type) {
    this->model = model;
    this->position = position;
    this->type = type;
    count++;
    id = count;
}
Object::Object(const char* modelpath, const char* texturepath, const glm::vec3& position, ObjectType type) {
    this->model = Model(modelpath);
    this->texture = imageBMP(texturepath);
    this->position = position;
    this->type = type;
    count++;
    id = count;
}
Object::Object(const char* modelpath, const glm::vec3& position, ObjectType type) {
    this->model = Model(modelpath);
    this->shader = std::make_shared<Shader>("./shaders/block.vert", "./shaders/no-texture.frag");

    count++;
    id = count;
}
bool Object::operator<(const Object& other) const {
    return id < other.id;
}
bool Object::operator==(const Object& other) const {
    return id == other.id;
}
bool Object::operator!=(const Object& other) const {
    return id != other.id;
}

void Object::render(const ShaderOptions& options) const {

    this->shader->drawObjectInstaced(this->model, this->texture, options);
};

glm::vec3 Object::getPosition() const {
    return this->position;
};
void Object::setPosition(const glm::vec3& position) {
    this->position = position;
};

void Object::move(const glm::vec3 &position)
{
    this->position += position;
}

void Object::setShader(std::shared_ptr<Shader> shader)
{
    this->shader = shader;
}