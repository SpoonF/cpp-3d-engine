#include "engine/entity/block.h"


Block::Block(const Object3D& object, const glm::vec3& position) 
: Entity(object, position, EntityType::BLOCK), CollisionBox(object, position, glm::vec3(2.f)) {}

Block::Block(const glm::vec3& position) 
: Entity(position, EntityType::BLOCK), CollisionBox(position, glm::vec3(2.f)) {}

void Block::setPosition(const glm::vec3& position) {
    Entity::setPosition(position);
    CollisionBox::setPosition(position);
}


Slab::Slab(const Object3D& object, const glm::vec3& position) 
: Block(object, position), Entity(object, position, EntityType::SLAB), CollisionBox(object, position, glm::vec3(2.f, 1.f, 2.f)) {}

LightBlock::LightBlock(const Object3D& object, const glm::vec3& position) 
: Block(object, position), Entity(object, position, EntityType::LIGHT), CollisionBox(object, position, glm::vec3(2.f)) {}