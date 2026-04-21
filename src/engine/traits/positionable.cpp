#include "engine/traits/positionable.h"

Positionable::Positionable(const glm::vec3& position) {
    this->position = position;
}
glm::vec3 Positionable::getPosition() const {
    return this->position;
}
void Positionable::setPosition(const glm::vec3& position) {
    this->position = position;
}
void Positionable::move(const glm::vec3& position) {
    this->position += position;
}
