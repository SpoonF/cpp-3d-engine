#include "engine/object.h"
#include "engine/shader.h"
#include "utils/imageBMP.h"
#include "utils/model.h"

std::unique_ptr<Model> Object::model = nullptr;
std::unique_ptr<imageBMP> Object::texture = nullptr;


Object::Object(const char* modelpath, const char* texturepath, const glm::vec3& position, ObjectType type) {

    printf("Create object");
    this->model = std::make_unique<Model>(modelpath);
    this->texture = std::make_unique<imageBMP>(texturepath);
    this->position = position;
    this->type = type;

    count++;
    id = count;
}
Object::Object(const char* modelpath, const glm::vec3& position, ObjectType type) {
        printf("Create object");
    this->model = std::make_unique<Model>(modelpath);
    this->position = position;
    this->type = type;

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

int Object::getId()
{
    return this->id;
}

Model *Object::getModel()
{
    return model.get();
}
imageBMP* Object::getTexture()
{
    return texture.get();
}

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

ObjectType Object::getType() const
{
    return this->type;
}