#include "collision.h"
#include <cmath>
#include <glm/ext/quaternion_geometric.hpp>

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



CollisionSphere::CollisionSphere(glm::vec3 position, float radius): Collision(position) {
    this->radius = radius;
}
CollisionSphere::CollisionSphere(const Object3D& object, glm::vec3 position, float radius): Collision(object, position) {
    this->radius = radius;
}
bool CollisionSphere::checkCollision(CollisionSphere* other, CollisionInfo& info) {
    auto getDirection = [](glm::vec3 delta){
        glm::vec3 direction{0, 0, 0};
        size_t bigIndex = 0;
        for (size_t i = 1; i < 3; i++)
        {
            if(fabs(delta[bigIndex]) < fabs(delta[i])) {
                bigIndex = i;
            }
        }
        direction[bigIndex] = delta[bigIndex];

        return direction;
    };

    glm::vec3 delta = other->getPosition() - this->getPosition();

    if(radius == 0.f) {
        radius = this->radius;
    }
    
    float distance = glm::length(delta);
    float radiusSum = radius + other->radius;
    
    if (distance < radiusSum) {
        // Есть коллизия
        info.Normal = glm::normalize(delta);
        info.PenetrationDepth = radiusSum - distance;
        info.direction = getDirection(delta);
        info.collisionPoints.push_back(
            this->getPosition() + info.Normal
        );

        // printf("[debag]: direction data x: %f, y: %f, z: %f \n", info.direction.x, info.direction.y, info.direction.z);
        return true;
    }
    return false;
}




CollisionBox::CollisionBox(glm::vec3 position, glm::vec3 size): Collision(position) {
    this->size = size;
}
CollisionBox::CollisionBox(const Object3D& object, glm::vec3 position, glm::vec3 size): Collision(object, position) {
    this->size = size;
}
void CollisionBox::setSize(glm::vec3 size) {
    this->size = size;
};
glm::vec3 CollisionBox::getSize() {
    return this->size;
};
bool CollisionBox::checkCollision(CollisionBox* other, CollisionInfo& info) {
    auto getDirection = [](glm::vec3 delta){
        glm::vec3 direction{0, 0, 0};
        size_t bigIndex = 0;
        for (size_t i = 1; i < 3; i++)
        {
            if(fabs(delta[bigIndex]) < fabs(delta[i])) {
                bigIndex = i;
            }
        }
        direction[bigIndex] = delta[bigIndex];

        return direction;
    };

    glm::vec3 pos = this->getPosition();
    glm::vec3 size = this->getSize();
    glm::vec3 oPos = other->getPosition();
    glm::vec3 oSize = other->getSize();

    glm::vec3 delta = oPos - pos;
    
    bool collision = 
        (oPos.x <= (pos.x + size.x) && (oPos.x + oSize.x) >= pos.x) &&
        (oPos.y <= (pos.y + size.y) && (oPos.y + oSize.y) >= pos.y) &&
        (oPos.z <= (pos.z + size.z) && (oPos.z + oSize.z) >= pos.z);
    
    // printf("is collision: %i size: %f %f %f\n", collision, size.x, size.y, size.z);
    
    if (collision) {
        // Есть коллизия
        info.Normal = glm::normalize(delta);
        // info.PenetrationDepth = radiusSum - distance;
        info.direction = getDirection(delta);
        info.collisionPoints.push_back(
            this->getPosition() + info.Normal
        );

        // printf("[debag]: direction data x: %f, y: %f, z: %f \n", info.direction.x, info.direction.y, info.direction.z);
        return true;
    }
    return false;
}