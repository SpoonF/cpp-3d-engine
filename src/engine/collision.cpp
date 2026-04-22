#include "engine/collision.h"

Collision::Collision(const glm::vec3& position) : Positionable(position) {}

bool Collision::isWithinDistance(const glm::vec3 &pos, float distance) const {
    glm::vec3 dist = this->position - pos;

    return !(abs(dist.x) > distance || abs(dist.y) > distance || abs(dist.z) > distance);
}
