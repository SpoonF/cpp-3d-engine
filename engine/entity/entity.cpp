#include "entity.h"

Entity::Entity(const Object3D& object, glm::vec3 position, EntityType type) {
    this->position;
    this->object;
    this->type = type;
    this->count++;
    this->id = count;
}
bool Entity::constains(const glm::vec3& point) const {
    return glm::all(glm::greaterThanEqual(point, position)) && 
    glm::all(glm::lessThanEqual(point, position + 1.f));
}
void Entity::setRotate(glm::vec3 rotate) {
    this->rotate = rotate;
}
glm::vec3 Entity::getRotate() {
    return this->rotate;
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
void Entity::setPosition(glm::vec3 position) {
    this->position = position;
};

Block::Block(const Object3D& object, glm::vec3 position) 
: Entity(object, position, EntityType::BLOCK), CollisionBox(object, position, glm::vec3(2.f)) {}