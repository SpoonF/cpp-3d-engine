#include "engine/object.h"
#include "engine/shader.h"
#include "utils/imageBMP.h"
#include "utils/model.h"
#include <iostream>


Object::Object(const glm::vec3& position, ObjectType type): Positionable(position) {
    this->type = type;

    this->options = std::make_unique<Options>();

    count++;
    this->id = count;
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

Options *Object::getOptions()
{
    return this->options.get();
}


template <ObjectType type>
void Object::init(const char* modelpath, const char* texturepath, const char* vert, const char* frag) {
    Model::getInstance(modelpath, type);
    imageBMP::getInstance(texturepath, type);
    Shader::getInstance(vert, frag, type);
};
