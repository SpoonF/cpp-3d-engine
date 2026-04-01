#include "engine/entity.h"
#include "engine/object.h"

bool EntityOptions::isLighting() {
    return this->isLight;
}

Entity::Entity(const Object3D& object, const glm::vec3& position, EntityType type) {
    this->position = position;
    this->object = object;
    this->type = type;
    this->count++;
    this->id = count;
}
Entity::Entity(const glm::vec3& position, EntityType type) {
    this->position = position;
    this->object = {};
    this->type = type;
    this->count++;
    this->id = count;
}
bool Entity::constains(const glm::vec3& point) const {
    return glm::all(glm::greaterThanEqual(point, position)) && 
    glm::all(glm::lessThanEqual(point, position + 1.f));
}
Object3D Entity::getObject() {
    return this->object;
}
EntityType Entity::getType() {
    return this->type;
}
glm::vec3 Entity::getPosition() {
    return this->position;
};
void Entity::setPosition(const glm::vec3& position) {
    this->position = position;
};


