#include "engine/entity/player.h"

Player::Player(const Object3D& object, const glm::vec3& position) 
: Entity(object, position, EntityType::PLAYER), CollisionBox(object, position, glm::vec3(1.f, 3.7f, 1.f)) {}

void Player::setPosition(const glm::vec3& position) {
    Entity::setPosition(position);
    CollisionBox::setPosition(position);
}