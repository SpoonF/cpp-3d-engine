#include "entity.h"

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


Block::Block(const Object3D& object, const glm::vec3& position) 
: Entity(object, position, EntityType::BLOCK), CollisionBox(object, position, glm::vec3(2.f)) {}

void Block::setPosition(const glm::vec3& position) {
    Entity::setPosition(position);
    CollisionBox::setPosition(position);
}
Slab::Slab(const Object3D& object, const glm::vec3& position) 
: Block(object, position), Entity(object, position, EntityType::SLAB), CollisionBox(object, position, glm::vec3(2.f, 1.f, 2.f)) {}

Player::Player(const Object3D& object, const glm::vec3& position) 
: Entity(object, position, EntityType::PLAYER), CollisionBox(object, position, glm::vec3(1.f, 3.7f, 1.f)) {}

void Player::setPosition(const glm::vec3& position) {
    Entity::setPosition(position);
    CollisionBox::setPosition(position);
}

LightBlock::LightBlock(const Object3D& object, const glm::vec3& position) 
: Block(object, position), Entity(object, position, EntityType::LIGHT), CollisionBox(object, position, glm::vec3(2.f)) {}