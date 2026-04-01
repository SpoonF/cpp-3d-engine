#include "engine/collision.h"

#include <cmath>
#include <glm/ext/quaternion_geometric.hpp>

#include "engine/object.h"
#include "utils/model.h"

Collision::Collision(const Object3D& object, glm::vec3 position) {
    this->object = object;
    this->position = position;
}
Collision::Collision(glm::vec3 position) {
    this->position = position;
}
int Collision::getVertexCount() {
    return object.model->vertices.size();
};
glm::vec3 Collision::getVertex(int i) {
    return object.model->vertices[i];
};
void Collision::setPosition(const glm::vec3& pos) {
    this->position = pos;
};
glm::vec3 Collision::getPosition() {
    return position;
};