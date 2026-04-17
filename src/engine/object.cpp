#include "engine/object.h"
#include "engine/shader.h"
#include "utils/imageBMP.h"
#include "utils/model.h"

Object::Object(const glm::vec3& position, ObjectType type) {
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